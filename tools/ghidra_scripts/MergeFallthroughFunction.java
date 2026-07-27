//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class MergeFallthroughFunction extends GhidraScript {
  @Override
  public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length != 3) {
      throw new IllegalArgumentException(
          "usage: <primary-entry> <absorbed-entry> <inclusive-end>");
    }

    Address primaryAddress = parseHexAddress(args[0]);
    Address absorbedAddress = parseHexAddress(args[1]);
    Address endAddress = parseHexAddress(args[2]);
    Function primary = getFunctionAt(primaryAddress);
    Function absorbed = getFunctionAt(absorbedAddress);
    if (primary == null) {
      throw new IllegalStateException(
          "no primary function at " + primaryAddress);
    }
    if (absorbed == null) {
      throw new IllegalStateException(
          "no absorbed function at " + absorbedAddress);
    }

    AddressSet mergedBody =
        new AddressSet(primaryAddress, endAddress);
    FunctionIterator overlaps =
        currentProgram.getFunctionManager().getFunctions(
            mergedBody,
            true);
    while (overlaps.hasNext()) {
      Function function = overlaps.next();
      Address entry = function.getEntryPoint();
      if (!entry.equals(primaryAddress) &&
          !entry.equals(absorbedAddress)) {
        throw new IllegalStateException(
            "unexpected overlapping function " +
            function.getName(true) + " at " + entry);
      }
    }

    String absorbedName = absorbed.getName(true);
    removeFunctionAt(absorbedAddress);
    primary.setBody(mergedBody);
    primary.setComment(
        "Retail fallthrough body merged through " +
        endAddress + "; absorbed false split " +
        absorbedName + " at " + absorbedAddress + ".");
    println(
        "MERGED " + primary.getName(true) + " " +
        primaryAddress + "-" + endAddress +
        " absorbed=" + absorbedName + "@" +
        absorbedAddress + " length=" +
        primary.getBody().getNumAddresses());
  }

  private Address parseHexAddress(String text) {
    return toAddr(
        Long.parseLong(text.replace("0x", ""), 16));
  }
}
