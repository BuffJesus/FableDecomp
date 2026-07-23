import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.scalar.Scalar;
import java.util.*;

// Scan all instructions, track register values from lis/addi/addis/ori within
// a function, and report any load/store or address-form instruction whose
// computed effective address falls in [LO, HI). Bypasses Ghidra's reference DB
// (which misses PPC split-immediate computed addresses).
public class FindAccessors extends GhidraScript {
  long LO, HI;

  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    LO = Long.parseLong(a[0].replace("0x",""),16);
    HI = a.length>1 ? Long.parseLong(a[1].replace("0x",""),16) : LO+16;
    Listing lst = currentProgram.getListing();
    InstructionIterator it = lst.getInstructions(true);
    long[] reg = new long[64];
    boolean[] valid = new boolean[64];
    Function curf = null;
    int hits=0;
    boolean first=true;
    while (it.hasNext() && !monitor.isCancelled()) {
      Instruction ins = it.next();
      Function f = getFunctionContaining(ins.getAddress());
      if (f != curf || first) { curf=f; first=false; Arrays.fill(valid,false);
        // PPC: r0 reads as literal 0 in base/RA-of-addi slots.
        reg[0]=0; valid[0]=true; }
      String mn = ins.getMnemonicString();
      // update register tracking
      try {
        if (mn.equals("lis")) {
          int rd = regNum(ins,0); long v = scal(ins,1);
          if (rd>=0){ reg[rd]=(v<<16)&0xFFFFFFFFL; valid[rd]=true; }
        } else if (mn.equals("li")) {
          int rd = regNum(ins,0); long v = scal(ins,1);
          if (rd>=0){ reg[rd]=v&0xFFFFFFFFL; valid[rd]=true; }
        } else if (mn.equals("addi") || mn.equals("addic")) {
          int rd=regNum(ins,0), ra=regNum(ins,1); long v=scal(ins,2);
          if (rd>=0 && ra>=0 && valid[ra]){ reg[rd]=(reg[ra]+v)&0xFFFFFFFFL; valid[rd]=true; }
          else if(rd>=0) valid[rd]=false;
        } else if (mn.equals("addis")) {
          int rd=regNum(ins,0), ra=regNum(ins,1); long v=scal(ins,2);
          if (rd>=0 && ra>=0 && valid[ra]){ reg[rd]=(reg[ra]+((v<<16)&0xFFFFFFFFL))&0xFFFFFFFFL; valid[rd]=true; }
          else if(rd>=0) valid[rd]=false;
        } else if (mn.equals("ori")) {
          int rd=regNum(ins,0), ra=regNum(ins,1); long v=scal(ins,2);
          if (rd>=0 && ra>=0 && valid[ra]){ reg[rd]=(reg[ra]|(v&0xFFFF))&0xFFFFFFFFL; valid[rd]=true; }
          else if(rd>=0) valid[rd]=false;
        } else {
          // load/store forms: MN rT, disp(rA)
          boolean isMem = mn.startsWith("lwz")||mn.startsWith("stw")||mn.startsWith("lhz")||
                          mn.startsWith("lha")||mn.startsWith("sth")||mn.startsWith("lbz")||
                          mn.startsWith("stb")||mn.startsWith("lfs")||mn.startsWith("stfs")||
                          mn.startsWith("lfd")||mn.startsWith("stfd")||mn.startsWith("ld")||mn.startsWith("std");
          if (isMem) {
            // find the (disp, reg) memory operand
            int n = ins.getNumOperands();
            for (int op=0; op<n; op++) {
              Object[] objs = ins.getOpObjects(op);
              long disp=0; int base=-1; boolean hasReg=false, hasScalar=false;
              for (Object o: objs) {
                if (o instanceof Register){ base=((Register)o).getOffset()>=0?rnum((Register)o):-1; hasReg=true; }
                if (o instanceof Scalar){ disp=((Scalar)o).getSignedValue(); hasScalar=true; }
              }
              if (hasReg && base>=0 && valid[base]) {
                long ea=(reg[base]+disp)&0xFFFFFFFFL;
                if (ea>=LO && ea<HI) {
                  Function ff=getFunctionContaining(ins.getAddress());
                  println("HIT ea=0x"+Long.toHexString(ea)+"  "+mn+"  @"+ins.getAddress()+
                          "  in "+(ff!=null?ff.getName():"?")+"   | "+ins.toString());
                  hits++;
                }
              }
            }
          }
          // also catch addi that lands in range (address-taken)
        }
        // report addi/lis that directly compute an in-range address (address-of)
        if (mn.equals("addi")||mn.equals("addis")) {
          int rd=regNum(ins,0);
          if (rd>=0 && valid[rd] && reg[rd]>=LO && reg[rd]<HI) {
            Function ff=getFunctionContaining(ins.getAddress());
            println("ADDR-OF r"+rd+"=0x"+Long.toHexString(reg[rd])+"  @"+ins.getAddress()+
                    "  in "+(ff!=null?ff.getName():"?")+"   | "+ins.toString());
            hits++;
          }
        }
      } catch (Exception e) { /* skip */ }
    }
    println("TOTAL HITS="+hits+" range=0x"+Long.toHexString(LO)+"-0x"+Long.toHexString(HI));
  }

  int regNum(Instruction ins, int op){
    Object[] o=ins.getOpObjects(op);
    for(Object x:o) if(x instanceof Register) return rnum((Register)x);
    return -1;
  }
  int rnum(Register r){
    String n=r.getName();
    if(n.startsWith("r")){ try{return Integer.parseInt(n.substring(1));}catch(Exception e){return -1;} }
    return -1;
  }
  long scal(Instruction ins,int op){
    Object[] o=ins.getOpObjects(op);
    for(Object x:o) if(x instanceof Scalar) return ((Scalar)x).getSignedValue();
    return 0;
  }
}
