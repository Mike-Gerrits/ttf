#include "ttf.h"

#include <stdint.h>
#include <stdlib.h>

const char *platform[] = {"Unicode","Macintosh","ISO [deprecated]","Windows","Custom"};
const char *platform0[] = {
"Unicode 1.0 semantics—deprecated                          "
,"Unicode 1.1 semantics—deprecated                          "
,"ISO/IEC 10646 semantics—deprecated                        "
,"Unicode 2.0 and onwards semantics, Unicode BMP only       "
,"Unicode 2.0 and onwards semantics, Unicode full repertoire"
,"Unicode Variation Sequences—for use with subtable format 14"
,"Unicode full repertoire—for use with subtable format 13   "
};
const char *platform2[] = {
"   	7-bit ASCII   "
,"   	ISO 10646     "
,"  	ISO 8859-1    "
};
const char *platform3[] = {
"Symbol                        "
,"Unicode BMP                   "
,"ShiftJIS                      "
,"PRC                           "
,"Big5                          "
,"Wansung                       "
,"Johab                         "
,"Reserved                      "
,"Reserved                      "
,"Reserved                      "
,"	Unicode full repertoire   "
};

const char *paint_enum[] = {
"COMPOSITE_CLEAR                  "
,"COMPOSITE_SRC                    "
,"COMPOSITE_DEST                   "
,"COMPOSITE_SRC_OVER               "
,"COMPOSITE_DEST_OVER              "
,"COMPOSITE_SRC_IN                 "
,"COMPOSITE_DEST_IN                "
,"COMPOSITE_SRC_OUT                "
,"COMPOSITE_DEST_OUT               "
,"COMPOSITE_SRC_ATOP               "
,"COMPOSITE_DEST_ATOP              "
,"COMPOSITE_XOR                    "
,"COMPOSITE_PLUS                   "
,"Separable color blend modes:     "
,"COMPOSITE_SCREEN                 "
,"COMPOSITE_OVERLAY                "
,"COMPOSITE_DARKEN                 "
,"COMPOSITE_LIGHTEN                "
,"COMPOSITE_COLOR_DODGE            "
,"COMPOSITE_COLOR_BURN             "
,"COMPOSITE_HARD_LIGHT             "
,"COMPOSITE_SOFT_LIGHT             "
,"COMPOSITE_DIFFERENCE             "
,"COMPOSITE_EXCLUSION              "
,"COMPOSITE_MULTIPLY               "
,"Non-separable color blend modes: "
,"COMPOSITE_HSL_HUE                "
,"COMPOSITE_HSL_SATURATION         "
,"COMPOSITE_HSL_COLOR              "
,"COMPOSITE_HSL_LUMINOSITY         "
};


int main(){


    // zet alles in font
    TTF_Font font = *(TTF_Font *)(data);
    //printf("%x\n",font.scaler_type);
   // TTF_Table * tables;

    font.tables = (TTF_Table *) calloc(font.numables, sizeof(*font.tables));
    //lees de tables in
    for (uint16_t i = 0; i < font.numables; i++) {
          // font.tables[i] = *(TTF_Table *)(&font.tables[i] );
            font.tables[i] =  *(TTF_Table *)(data + 12 + sizeof(TTF_Table)*i);// de 12 komt van de grootte van TTF_Font maar zonder TTF_Table in het helaas zou het 12 moeten zijn maar kan het niet
           //dit is voor de union denk ik TODO: kijk hierna
            //table->status = 0;
          //tabel naam

    }


}
