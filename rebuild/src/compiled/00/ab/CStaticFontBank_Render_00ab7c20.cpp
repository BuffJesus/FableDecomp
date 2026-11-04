// Retail 00AB7C20; CPU vertex generation and dispatch qualified by the native render gate.
#include <math.h>
#include "fable_font_instances.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_font_vertex_buffer.h"
#include "fable_render_state.h"
#include "fable_2d_vector.h"
#include "fable_4d_vector.h"
#include "fable_rgb_colour.h"

struct FontVertex {
    C4DVector Position;
    unsigned long Colour;
    float U,V;
    FontVertex(const C4DVector& point,unsigned long colour,float u,float v)
        :Position(point),Colour(colour),U(u),V(v){}
};
FABLE_STATIC_ASSERT(sizeof(FontVertex)==28);

static __forceinline void SelectFontBuffer(CRenderManagerCore* render,CVertexBufferWin32* buffer)
{
    if(buffer){
        FableRenderStateDevice* device=reinterpret_cast<FableRenderStateDevice*>(render->PD3DDevice);
        device->vtable->SetStreamSource(device,0,buffer->Buffer,0,buffer->VertexStride);
    }else{
        FableRenderStateDevice* device=reinterpret_cast<FableRenderStateDevice*>(render->PD3DDevice);
        device->vtable->SetStreamSource(device,0,0,0,0);
    }
}

void CStaticFontBank::Render(const CWideString& text,const C3DVector& position,float scale,
    const CRGBColour& colour,unsigned long flags,const CTexture* texture) const
{
    float anchorX=position.X,anchorY=position.Y;
    float penX=anchorX,penY=anchorY;
    const unsigned long packedColour=*reinterpret_cast<const unsigned long*>(&colour);
    CRenderManagerCore* render=&GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    const CTexture* textures[2]={PTexture,texture};
    unsigned long textureCount=texture?2:1;
    CVertexBufferWin32* buffer=&PManager->GetFreeVertexBuffer();
    if(flags&8)penY=static_cast<float>(floor(anchorY-GetStringExtents(text,false).Y*0.5f));
    FontVertex* vertices=reinterpret_cast<FontVertex*>(buffer->Lock(0,0,0));
    unsigned long triangles=0;
    long lineStart=1;
    const wchar_t* chars=text.GetAsCharArray();
    while(*chars){
        if(lineStart){
            if(flags&6){
                float width=GetStringExtents(chars,true).X;
                if(flags&2)penX=static_cast<float>(floor(anchorX-width));
                if(flags&4)penX=static_cast<float>(floor(anchorX-width*0.5f));
            }
            lineStart=0;
        }
        wchar_t character=*chars++;
        if(character==10){penX=anchorX;penY+=static_cast<float>(FontHeight);lineStart=1;}
        if(static_cast<long>(character)<MinChar || static_cast<long>(character)>MaxChar)continue;
        const CStaticFontGlyphBank& bank=GlyphBanks[character>>6];
        if(!bank.Glyphs)continue;
        unsigned index=(character&63)-bank.FirstGlyph;
        if(index>=bank.NoGlyphs)continue;
        const CGlyph* glyph=&bank.Glyphs[index];
        if(!glyph)continue;
        penX+=glyph->Offset;
        float left=anchorX+(penX-anchorX)*scale-0.5f;
        // Original rounds left to float for the width addition, while retaining its working value.
        const volatile float* storedLeft=&left;
        float right=*storedLeft+(glyph->Width+1.0f)*scale;
        float top=anchorY+(penY-anchorY)*scale-0.5f;
        float bottom=top+(static_cast<float>(FontHeight)+1.0f)*scale;
        penX+=glyph->Advance;
        float uLeft=glyph->Left*(TextureSize_X-1)/TextureSize_X;
        float vTop=glyph->Top*(TextureSize_Y-1)/TextureSize_Y;
        float uRight=glyph->Right*(TextureSize_X-1)/TextureSize_X;
        float vBottom=glyph->Bottom*(TextureSize_Y-1)/TextureSize_Y;
        *vertices++=FontVertex(C4DVector(left,bottom,position.Z,1.0f),packedColour,uLeft,vBottom);
        *vertices++=FontVertex(C4DVector(left,top,position.Z,1.0f),packedColour,uLeft,vTop);
        *vertices++=FontVertex(C4DVector(right,bottom,position.Z,1.0f),packedColour,uRight,vBottom);
        *vertices++=FontVertex(C4DVector(right,top,position.Z,1.0f),packedColour,uRight,vTop);
        *vertices++=FontVertex(C4DVector(right,bottom,position.Z,1.0f),packedColour,uRight,vBottom);
        *vertices++=FontVertex(C4DVector(left,top,position.Z,1.0f),packedColour,uLeft,vTop);
        triangles+=2;
        if(triangles*3>294){
            buffer->Unlock();SelectFontBuffer(render,buffer);
            render->DrawCurrentBuffersAsTris(textures,textureCount,0,triangles);
            SelectFontBuffer(render,0);
            buffer=&PManager->GetFreeVertexBuffer();
            vertices=reinterpret_cast<FontVertex*>(buffer->Lock(0,0,0));
            triangles=0;
        }
    }
    buffer->Unlock();
    if(triangles){
        SelectFontBuffer(render,buffer);
        render->DrawCurrentBuffersAsTris(textures,textureCount,0,triangles);
        SelectFontBuffer(render,0);
    }
}
