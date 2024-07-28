
#include <uchar.h>
#pragma scalar_storage_order big-endian
extern unsigned char data[42756];

#include <stdint.h>
#include <stddef.h>

#define TAG(name) ((uint32)(((name[0] << 24) | (name[1] << 16) | (name[2] << 8) | name[3])))
#define UNTAG(value)  \
({                                 \
    char result[4];                   \
    result[0] = (value >> 24) & 0xFF; \
    result[1] = (value >> 16) & 0xFF; \
    result[2] = (value >> 8) & 0xFF;  \
    result[3] = value & 0xFF;        \
    result;                           \
  })
//om het makkelijk te maken
//*signed*//
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
//*unsigned*//
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int16 FWORD;
typedef uint16 UFWORD;

typedef uint16 Offset16;
typedef uint32 Offset32;

typedef uint8 Tag[4];

typedef uint16 Version16Dot16[2];
//tabellen begin

typedef struct _Fixed {
	int16 integer;
	int16 decimal;
} Fixed;

//SbitLineMetrics//
typedef struct _SbitLineMetrics{
int8 	ascender;
int8 	descender;
uint8 	widthMax;
int8 	caretSlopeNumerator;
int8 	caretSlopeDenominator;
int8 	caretOffset;
int8 	minOriginSB;
int8 	minAdvanceSB;
int8 	maxBeforeBL;
int8 	minAfterBL;
int8 	pad1;
int8 	pad2;
} SbitLineMetrics;

//avar — Axis Variations Table//
//The F2DOT14 format consists of a signed, 2’s complement integer and an unsigned fraction. To compute the actual value, take the integer and add the fraction. Examples of 2.14 values are:

// Decimal Value 	Hex Value 	Integer 	Fraction
// 1.999939 	0x7fff 	1 	16383/16384
// 1.75 	0x7000 	1 	12288/16384
// 0.000061 	0x0001 	0 	1/16384
// 0.0 	0x0000 	0 	0/16384
// -0.000061 	0xffff 	-1 	16383/16384
// -2.0 	0x8000 	-2 	0/16384

typedef struct _F2DOT14{
	uint8 number:2;//nummer plus
	uint16 fraction:14;// fraction/16384
} F2DOT14;

typedef F2DOT14 * Tuple;// 	Coordinate array specifying a position within the font’s variation space. The number of elements must match the axisCount specified in the 'fvar' table.
typedef struct _AxisValueMap {
F2DOT14 	fromCoordinate 	;//A normalized coordinate value obtained using default normalization.
F2DOT14 	toCoordinate 	;//The modified, normalized coordinate value.
} AxisValueMap;

typedef struct _SegmentMaps {
uint16 	positionMapCount ;//	The number of correspondence pairs for this axis.
AxisValueMap *axisValueMaps; // 	The array of axis value map records for this axis.
} SegmentMaps;


typedef struct _avar_Tabel {
uint16 	majorVersion; //	Major version number of the axis variations table — set to 1.
uint16 	minorVersion; //	Minor version number of the axis variations table — set to 0.
uint16 	reserved; //	Permanently reserved; set to zero.
uint16 	axisCount; //	The number of variation axes for this font. This must be the same number as axisCount in the 'fvar' table.
SegmentMaps  *axisSegmentMaps;//	The segment maps array — one segment map for each axis, in the order of axes specified in the 'fvar' table.
} avar_Tabel;

//Base versie 1 heeft geen itemVarStoreOffset
typedef struct _base_Header{
uint16 	majorVersion; //	Major version of the BASE table, = 1
uint16 	minorVersion; //	Minor version of the BASE table, = 0
uint16 	horizAxisOffset; //	Offset to horizontal Axis table, from beginning of BASE table (may be NULL)
uint16 	vertAxisOffset; //	Offset to vertical Axis table, from beginning of BASE table (may be NULL)
uint32 	itemVarStoreOffset;// 	Offset to Item Variation Store table, from beginning of BASE table (may be null)
}base_Header;

typedef struct _Axis_Table{
uint16 	baseTagListOffset ;//	Offset to BaseTagList table, from beginning of Axis table (may be NULL)
uint16 	baseScriptListOffset ;//	Offset to BaseScriptList table, from beginning of Axis table
}Axis_Table;

typedef struct _BaseTagList{
uint16 	baseTagCount;// 	Number of baseline identification tags in this text direction — may be zero (0)
uint32 	* baselineTags;// 	Array of 4-byte baseline identification tags — must be in alphabetical order
}BaseTagList;

typedef struct _BaseScriptRecord{
uint32 	baseScriptTag;// 	4-byte script identification tag
uint16 	baseScriptOffset;// 	Offset to BaseScript table, from beginning of BaseScriptList
}BaseScriptRecord;

typedef struct _BaseScriptList{
uint16 	baseScriptCount;// 	Number of BaseScriptRecords defined
BaseScriptRecord 	*baseScriptRecords ;//	Array of BaseScriptRecords, in alphabetical order by baseScriptTag
}BaseScriptList;

typedef struct _BaseLangSysRecord{
uint32 	baseLangSysTag;// 	4-byte language system identification tag
uint16 	minMaxOffset;// 	Offset to MinMax table, from beginning of BaseScript table
}BaseLangSysRecord;

typedef struct _BaseScript_Table{
uint16 	baseValuesOffset 	;//Offset to BaseValues table, from beginning of BaseScript table (may be NULL)
uint16 	defaultMinMaxOffset ;//	Offset to MinMax table, from beginning of BaseScript table (may be NULL)
uint16 	baseLangSysCount;// 	Number of BaseLangSysRecords defined — may be zero (0)
BaseLangSysRecord *	baseLangSysRecords ;//	Array of BaseLangSysRecords, in alphabetical order by BaseLangSysTag
}BaseScript_Table;

typedef struct _BaseValuesable{
uint16 	defaultBaselineIndex; //	Index number of default baseline for this script — equals index position of baseline tag in baselineTags array of the BaseTagList
uint16 	baseCoordCount;// 	Number of BaseCoord tables defined — should equal baseTagCount in the BaseTagList
uint16 	*baseCoordOffsets;// 	Array of offsets to BaseCoord tables, from beginning of BaseValues table — order matches baselineTags array in the BaseTagList
}BaseValuesable;

typedef struct _FeatMinMaxRecord{
uint32 	featureTableTag;// 	4-byte feature identification tag — must match feature tag in FeatureList
uint8 	minCoordOffset;// 	Offset to BaseCoord table that defines the minimum extent value, from beginning of MinMax table (may be NULL)
uint8 	maxCoordOffset;// 	Offset to BaseCoord table that defines the maximum extent value, from beginning of MinMax table (may be NULL)
}FeatMinMaxRecord;

typedef struct _MinMaxable{
uint16 	minCoordOffset ;//	Offset to BaseCoord table that defines the minimum extent value, from the beginning of MinMax table (may be NULL)
uint16 	maxCoordOffset ;//	Offset to BaseCoord table that defines maximum extent value, from the beginning of MinMax table (may be NULL)
uint16 	featMinMaxCount ;//	Number of FeatMinMaxRecords — may be zero (0)
FeatMinMaxRecord *	featMinMaxRecords;// 	Array of FeatMinMaxRecords, in alphabetical order by featureTableTag
}MinMaxable ;

typedef struct _BaseCoordable{
uint16 	baseCoordFormat ;//	Format identifier — format = 1 then next format = 2 then add two uint16 and format = 3 add uint16
int16 	coordinate ;//	X or Y value, in design units
}BaseCoordable;

typedef struct _BaseCoordable_format_2{
uint16 	referenceGlyph;// 	Glyph ID of control glyph
uint16 	baseCoordPoint;// 	Index of contour point on the reference glyph
}BaseCoordable_format_2;

typedef struct _BaseCoordable_format_3{
uint16 	deviceOffset;// 	Offset to Device table (non-variable font) / Variation Index table (variable font) for X or Y value, from beginning of BaseCoord table (may be NULL).
}BaseCoordable_format_3;

//CBDT — Color Bitmap Data Table
typedef struct _CBDTable{
uint16 	majorVersion;// 	Major version of the CBDT table, = 3.
uint16 	minorVersion;// 	Minor version of the CBDT table, = 0.
} CBDTable;

typedef struct _BigGlyphMetrics{
uint8 	height;// 	Number of rows of data.
uint8 	width;// 	Number of columns of data.
int8 	horiBearingX;// 	Distance in pixels from the horizontal origin to the left edge of the bitmap.
int8 	horiBearingY;// 	Distance in pixels from the horizontal origin to the top edge of the bitmap.
uint8 	horiAdvance ;//	Horizontal advance width in pixels.
int8 	vertBearingX;// 	Distance in pixels from the vertical origin to the left edge of the bitmap.
int8 	vertBearingY ;//	Distance in pixels from the vertical origin to the top edge of the bitmap.
uint8 	vertAdvance ;//	Vertical advance width in pixels.
}BigGlyphMetrics;

typedef struct _SmallGlyphMetrics{
uint8 	height;// 	Number of rows of data.
uint8 	width;// 	Number of columns of data.
int8 	bearingX;// 	Distance in pixels from the horizontal origin to the left edge of the bitmap (for horizontal text); or distance in pixels from the vertical origin to the top edge of the bitmap (for vertical text).
int8 	bearingY;// 	Distance in pixels from the horizontal origin to the top edge of the bitmap (for horizontal text); or distance in pixels from the vertical origin to the left edge of the bitmap (for vertical text).
uint8 	advance;// 	Horizontal or vertical advance width in pixels.
}SmallGlyphMetrics;

typedef struct _format_17{
SmallGlyphMetrics 	glyphMetrics;// 	Metrics information for the glyph
uint32 	dataLen;// 	Length of data in bytes
uint8 	*data;// 	Raw PNG data
}format_17 ;

typedef struct _format_18{
BigGlyphMetrics 	glyphMetrics;// 	Metrics information for the glyph
uint32 	dataLen;// 	Length of data in bytes
uint8 	*data;// 	Raw PNG data
}format_18 ;

typedef struct _format_19{
uint32 	dataLen;// 	Length of data in bytes
uint8 	*data;// 	Raw PNG data
}format_19 ;

//CBLC — Color Bitmap Location Table
typedef struct _BitmapSize_Record{
uint32 	indexSubTableArrayOffset;// 	Offset to index subtable from beginning of CBLC.
uint32 	indexTablesSize;// 	Number of bytes in corresponding index subtables and array.
uint32 	numberOfIndexSubTables;// 	There is an index subtable for each range or format change.
uint32 	colorRef;// 	Not used; set to 0.
SbitLineMetrics 	hori;// 	Line metrics for text rendered horizontally.
SbitLineMetrics 	vert;// 	Line metrics for text rendered vertically.
uint16 	startGlyphIndex;// 	Lowest glyph index for this size.
uint16 	endGlyphIndex;// 	Highest glyph index for this size.
uint8 	ppemX ;//	Horizontal pixels per em.
uint8 	ppemY ;//	Vertical pixels per em.
uint8 	bitDepth;// 	In addtition to already defined bitDepth values 1, 2, 4, and 8 supported by existing implementations, the value of 32 is used to identify color bitmaps with 8 bit per pixel RGBA channels.
int8 	flags;// 	Vertical or horizontal (see the Bitmap Flags section of the EBLC table chapter).
}BitmapSize_Record ;


typedef struct _CblcHeader{
uint16 	majorVersion;// 	Major version of the CBLC table, = 3.
uint16 	minorVersion;// 	Minor version of the CBLC table, = 0.
uint32 	numSizes;// 	Number of BitmapSize records.
BitmapSize_Record 	bitmapSizes;// 	BitmapSize records array.
}CblcHeader ;

//cff1 weet ik niet

//CFF2 — Compact Font Format (CFF) Version 2
typedef struct _CFF2Header{
uint8 	majorVersion;// 	Format major version. Set to 2.
uint8 	minorVersion;// 	Format minor version. Set to zero.
uint8 	headerSize;// 	Header size (bytes).
uint16 	topDictLength;// 	Length of Top DICT structure in bytes
} CFF2Header;
//TODO: klote cff weet ik veel

//cmap — Character to Glyph Index Mapping Table
typedef struct _EncodingRecord{
uint16 	platformID ;//	Platform ID.
uint16 	encodingID ;//	Platform-specific encoding ID.
uint32 	subtableOffset;// 	Byte offset from beginning of table to the subtable for this encoding.
}EncodingRecord;

extern const char *platform[] ;
extern const char *platform0[] ;
extern const char *platform1[] ;
extern const char *platform3[] ;

typedef struct subtable_format_0{
uint16 	format;// 	Format number is set to 0.
uint16 	length;// 	This is the length in bytes of the subtable.
uint16 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint8 	glyphIdArray[256];// 	An array that maps character codes to glyph index values.
} cmap_Subtable0;

typedef struct _SubHeader{
uint16 	firstCode;// 	First valid low byte for this SubHeader.
uint16 	entryCount;// 	Number of valid low bytes for this SubHeader.
int16 	idDelta;// 	See text below.
uint16 	idRangeOffset;// 	See text below.
}SubHeader ;

typedef struct subtable_format_1{
uint16 	format ;//	Format number is set to 2.
uint16 	length 	;//This is the length in bytes of the subtable.
uint16 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint16 	subHeaderKeys[256];// 	Array that maps high bytes to subHeaders: value is subHeader index × 8.
SubHeader 	*subHeaders;// 	Variable-length array of SubHeader records.
uint16 	*glyphIdArray;// 	Variable-length array containing subarrays used for mapping the low byte of 2-byte characters.
} cmap_Subtable1;

typedef struct subtable_format_4{
uint16 	format ;	//Format number is set to 4.
uint16 	length;// 	This is the length in bytes of the subtable.
uint16 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint16 	segCountX2;// 	2 × segCount.
uint16 	searchRange;// 	Maximum power of 2 less than or equal to segCount, times 2 ((2**floor(log2(segCount))) * 2, where “**” is an exponentiation operator)
uint16 	entrySelector;// 	Log2 of the maximum power of 2 less than or equal to segCount (log2(searchRange/2), which is equal to floor(log2(segCount)))
uint16 	rangeShift;// 	segCount times 2, minus searchRange ((segCount * 2) - searchRange)
uint16 	*endCode;//[segCount] 	End characterCode for each segment, last=0xFFFF.
uint16 	reservedPad;// 	Set to 0.
uint16 	*startCode;//[segCount] 	Start character code for each segment.
int16 	*idDelta;//[segCount] 	Delta for all character codes in segment.
uint16 	*idRangeOffsets;//[segCount] 	Offsets into glyphIdArray or 0
uint16 	*glyphIdArray;//[ ] 	Glyph index array (arbitrary length)
} cmap_Subtable4;

typedef struct subtable_format_6{
uint16 	format;// 	Format number is set to 6.
uint16 	length;// 	This is the length in bytes of the subtable.
uint16 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint16 	firstCode;// 	First character code of subrange.
uint16 	entryCount;// 	Number of character codes in subrange.
uint16 *	glyphIdArray;//[entryCount] 	Array of glyph index values for character codes in the range.
} cmap_Subtable6;

typedef struct _SequentialMapGroup{
uint32 	startCharCode;// 	First character code in this group; note that if this group is for one or more 16-bit character codes (which is determined from the is32 array), this 32-bit value will have the high 16-bits set to zero
uint32 	endCharCode;// 	Last character code in this group; same condition as listed above for the startCharCode
uint32 	startGlyphID;// 	Glyph index corresponding to the starting character code
} SequentialMapGroup;

typedef struct subtable_format_8{
uint16 	format;// 	Subtable format; set to 8.
uint16 	reserved;// 	Reserved; set to 0
uint32 	length;// 	Byte length of this subtable (including the header)
uint32 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint8 	is32[8192];// 	Tightly packed array of bits (8K bytes total) indicating whether the particular 16-bit (index) value is the start of a 32-bit character code
uint32 	numGroups;// 	Number of groupings which follow
SequentialMapGroup 	*groups;//[numGroups] 	Array of SequentialMapGroup records.
} cmap_Subtable8;


typedef struct subtable_format_10{
uint16 	format;// 	Subtable format; set to 10.
uint16 	reserved;// 	Reserved; set to 0
uint32 	length;// 	Byte length of this subtable (including the header)
uint32 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint32 	startCharCode;// 	First character code covered
uint32 	numChars;// 	Number of character codes covered
uint16 	glyphIdArray[];// 	Array of glyph indices for the character codes covered
} cmap_Subtable10;

typedef struct subtable_format_12{
uint16 	format;// 	Subtable format; set to 12.
uint16 	reserved;// 	Reserved; set to 0
uint32 	length;// 	Byte length of this subtable (including the header)
uint32 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint32 	numGroups;// 	Number of groupings which follow
SequentialMapGroup 	groups;//[numGroups] 	Array of SequentialMapGroup records.
} cmap_Subtable12;

typedef  struct _ConstantMapGroup{
uint32 	startCharCode;// 	First character code in this group
uint32 	endCharCode;// 	Last character code in this group
uint32 	glyphID;// 	Glyph index to be used for all the characters in the group’s range.
}ConstantMapGroup;

typedef struct subtable_format_13{
uint16 	format;// 	Subtable format; set to 13.
uint16 	reserved;// 	Reserved; set to 0
uint32 	length;// 	Byte length of this subtable (including the header)
uint32 	language;// 	For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this document.
uint32 	numGroups;// 	Number of groupings which follow
ConstantMapGroup *	groups;//[numGroups] 	Array of ConstantMapGroup records.
} cmap_Subtable13;

typedef  struct _VariationSelector{
uint32 	varSelector:24;// 	Variation selector
uint32 	defaultUVSOffset;// 	Offset from the start of the format 14 subtable to Default UVS Table. May be 0.
uint32 	nonDefaultUVSOffset;// 	Offset from the start of the format 14 subtable to Non-Default UVS Table. May be 0.
}VariationSelector;

typedef struct subtable_format_14{
uint16 	format;// 	Subtable format. Set to 14.
uint32 	length;// 	Byte length of this subtable (including this header)
uint32 	numVarSelectorRecords;// 	Number of variation Selector Records
VariationSelector 	*varSelector;//[numVarSelectorRecords] 	Array of VariationSelector records.
} cmap_Subtable14;

typedef struct _UVSMapping{
uint32 	unicodeValue:24;// 	Base Unicode value of the UVS
uint16 	glyphID;// 	Glyph ID of the UVS
} UVSMapping;

typedef struct _cmap_non_uvs{
uint32 	numUVSMappings;// 	Number of UVS Mappings that follow
UVSMapping 	*uvsMappings;//[numUVSMappings] 	Array of UVSMapping records.
} cmap_non_uvs;

typedef struct _UnicodeRange{
uint32 	startUnicodeValue:24;// 	First value in this range
uint8 	additionalCount;// 	Number of additional values in this range
} UnicodeRange;

typedef struct _cmap_uvs{
uint32 	numUnicodeValueRanges;// 	Number of Unicode character ranges.
UnicodeRange 	*ranges;//[numUnicodeValueRanges] 	Array of UnicodeRange records.
} cmap_uvs;

typedef struct _cmapHeader{
uint16 	version;// 	Table version number (0).
uint16 	numTables;// 	Number of encoding tables that follow.
EncodingRecord 	*encodingRecords;//
} cmapHeader;

//COLR — Color Table
typedef struct _COLR_Header_version_0{
uint16 	version ;//	Table version number—set to 0.
uint16 	numBaseGlyphRecords;// 	Number of BaseGlyph records.
uint32 	baseGlyphRecordsOffset;// 	Offset to baseGlyphRecords array.
uint32 	layerRecordsOffset;// 	Offset to layerRecords array.
uint16 	numLayerRecords;// 	Number of Layer records.
}COLR_Header0 ;

typedef struct _COLR_Header_version_1{
uint16 	version;// 	Table version number—set to 1.
uint16 	numBaseGlyphRecords;// 	Number of BaseGlyph records; may be 0 in a version 1 table.
uint32 	baseGlyphRecordsOffset;// 	Offset to baseGlyphRecords array (may be NULL).
uint32 	layerRecordsOffset;// 	Offset to layerRecords array (may be NULL).
uint16 	numLayerRecords;// 	Number of Layer records; may be 0 in a version 1 table.
uint32 	baseGlyphListOffset;// 	Offset to BaseGlyphList table.
uint32 	layerListOffset ;//	Offset to LayerList table (may be NULL).
uint32 	clipListOffset;// 	Offset to ClipList table (may be NULL).
uint32 	varIndexMapOffset;// 	Offset to DeltaSetIndexMap table (may be NULL).
uint32 	itemVariationStoreOffset;// 	Offset to ItemVariationStore (may be NULL).
}COLR_Header1 ;

typedef struct _BaseGlyph_record{
uint16 	glyphID ;//	Glyph ID of the base glyph.
uint16 	firstLayerIndex;// 	Index (base 0) into the layerRecords array.
uint16 	numLayers ;//	Number of color layers associated with this glyph.
}BaseGlyph_record ;

typedef struct _Layer_record{
uint16 	glyphID ;//	Glyph ID of the glyph used for a given layer.
uint16 	paletteIndex;// 	Index (base 0) for a palette entry in the CPAL table.
}Layer_record ;

typedef struct _BaseGlyphPaintRecord{
uint16 	glyphID;// 	Glyph ID of the base glyph.
uint32 	paintOffset;// 	Offset to a Paint table.
} BaseGlyphPaintRecord;

typedef struct _BaseGlyphList{
uint32 	numBaseGlyphPaintRecords ;//
BaseGlyphPaintRecord 	*baseGlyphPaintRecords;//[numBaseGlyphPaintRecords]
} BaseGlyphList;

typedef struct _LayerList{
uint32 	numLayers;//
uint32 	*paintOffsets;//[numLayers] 	Offsets to Paint tables.
}LayerList ;


typedef struct _Clip{
uint16 	startGlyphID;// 	First glyph ID in the range.
uint16 	endGlyphID;// 	Last glyph ID in the range.
uint32 	clipBoxOffset:24;// 	Offset to a ClipBox table.
}Clip ;

typedef struct _ClipList{
uint8 	format;// 	Set to 1.
uint32 	numClips;// 	Number of Clip records.
Clip 	*clips;//[numClips] 	Clip records. Sorted by startGlyphID.
}ClipList ;


typedef struct _ClipBoxFormat1{
uint8 	format;// 	Set to 1.
FWORD 	xMin;// 	Minimum x of clip box.
FWORD 	yMin;// 	Minimum y of clip box.
FWORD 	xMax;// 	Maximum x of clip box.
FWORD 	yMax;// 	Maximum y of clip box.
}ClipBoxFormat1;

typedef struct _ClipBoxFormat2{
uint8 	format;// 	Set to 2.
FWORD 	xMin;// 	Minimum x of clip box. For variation, use varIndexBase + 0.
FWORD 	yMin;// 	Minimum y of clip box. For variation, use varIndexBase + 1.
FWORD 	xMax;// 	Maximum x of clip box. For variation, use varIndexBase + 2.
FWORD 	yMax;// 	Maximum y of clip box. For variation, use varIndexBase + 3.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}ClipBoxFormat2;

typedef struct _ColorIndex{
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value.
}ColorIndex;

typedef struct _VarColorIndex{
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value. For variation, use varIndexBase + 0.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}VarColorIndex;

typedef struct _ColorStop{
F2DOT14 	stopOffset;// 	Position on a color line.
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value.
}ColorStop;

typedef struct _VarColorStop{
F2DOT14 	stopOffset;// 	Position on a color line. For variation, use varIndexBase + 0.
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value. For variation, use varIndexBase + 1.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}VarColorStop;

typedef struct _ColorLine{
uint8 	extend;// 	An Extend enum value.
uint16 	numStops;// 	Number of ColorStop records.
ColorStop 	*colorStops;//[numStops]
}ColorLine;

typedef struct _VarColorLine{
uint8 	extend;// 	An Extend enum value.
uint16 	numStops;// 	Number of ColorStop records.
VarColorStop 	*colorStops;//[numStops] 	Allows for variations.
}VarColorLine;

typedef struct _PaintColrLayers{
uint8 	format;// 	Set to 1.
uint8 	numLayers;// 	Number of offsets to paint tables to read from LayerList.
uint32 	firstLayerIndex;// 	Index (base 0) into the LayerList.
}PaintColrLayers;

typedef struct _PaintSolid{
uint8 	format;// 	Set to 2.
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value.
}PaintSolid;

typedef struct _PaintVarSolid{
uint8 	format;// 	Set to 3.
uint16 	paletteIndex;// 	Index for a CPAL palette entry.
F2DOT14 	alpha;// 	Alpha value. For variation, use varIndexBase + 0.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarSolid;

typedef struct _PaintLinearGradient{
uint8 	format;// 	Set to 4.
uint32 	colorLineOffset:24;// 	Offset to ColorLine table.
FWORD 	x0;// 	Start point (p₀) x coordinate.
FWORD 	y0;// 	Start point (p₀) y coordinate.
FWORD 	x1;// 	End point (p₁) x coordinate.
FWORD 	y1;// 	End point (p₁) y coordinate.
FWORD 	x2;// 	Rotation point (p₂) x coordinate.
FWORD 	y2;// 	Rotation point (p₂) y coordinate.
}PaintLinearGradient;

typedef struct _PaintVarLinearGradient{
uint8 	format;// 	Set to 5.
uint32 	colorLineOffset:24;// 	Offset to VarColorLine table.
FWORD 	x0;// 	Start point (p₀) x coordinate. For variation, use varIndexBase + 0.
FWORD 	y0;// 	Start point (p₀) y coordinate. For variation, use varIndexBase + 1.
FWORD 	x1;// 	End point (p₁) x coordinate. For variation, use varIndexBase + 2.
FWORD 	y1;// 	End point (p₁) y coordinate. For variation, use varIndexBase + 3.
FWORD 	x2;// 	Rotation point (p₂) x coordinate. For variation, use varIndexBase + 4.
FWORD 	y2;// 	Rotation point (p₂) y coordinate. For variation, use varIndexBase + 5.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarLinearGradient;

typedef struct _PaintRadialGradient{
uint8 	format;// 	Set to 6.
uint32 	colorLineOffset:24;// 	Offset to ColorLine table.
FWORD 	x0;// 	Start circle center x coordinate.
FWORD 	y0;// 	Start circle center y coordinate.
UFWORD 	radius0;// 	Start circle radius.
FWORD 	x1;// 	End circle center x coordinate.
FWORD 	y1;// 	End circle center y coordinate.
UFWORD 	radius1;// 	End circle radius.
}PaintRadialGradient;

typedef struct _PaintVarRadialGradient{
uint8 	format;// 	Set to 7.
uint32 	colorLineOffset:24;// 	Offset to VarColorLine table.
FWORD 	x0;// 	Start circle center x coordinate. For variation, use varIndexBase + 0.
FWORD 	y0;// 	Start circle center y coordinate. For variation, use varIndexBase + 1.
UFWORD 	radius0;// 	Start circle radius. For variation, use varIndexBase + 2.
FWORD 	x1;// 	End circle center x coordinate. For variation, use varIndexBase + 3.
FWORD 	y1;// 	End circle center y coordinate. For variation, use varIndexBase + 4.
UFWORD 	radius1;// 	End circle radius. For variation, use varIndexBase + 5.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarRadialGradient;

typedef struct _PaintSweepGradient{
uint8 	format;// 	Set to 8.
uint32 	colorLineOffset:24;// 	Offset to ColorLine table.
FWORD 	centerX;// 	Center x coordinate.
FWORD 	centerY;// 	Center y coordinate.
F2DOT14 	startAngle;// 	Start of the angular range of the gradient, 180° in counter-clockwise degrees per 1.0 of value.
F2DOT14 	endAngle;// 	End of the angular range of the gradient, 180° in counter-clockwise degrees per 1.0 of value.
}PaintSweepGradient;

typedef struct _PaintVarSweepGradient{
uint8 	format;//	Set to 9.
uint32 	colorLineOffset:24;// 	Offset to VarColorLine table.
FWORD 	centerX;// 	Center x coordinate. For variation, use varIndexBase + 0.
FWORD 	centerY;// 	Center y coordinate. For variation, use varIndexBase + 1.
F2DOT14 	startAngle;// 	Start of the angular range of the gradient, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 2.
F2DOT14 	endAngle;// 	End of the angular range of the gradient, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 3.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarSweepGradient;


typedef struct _PaintGlyph{
uint8 	format;// 	Set to 10.
uint32 	paintOffset:24;// 	Offset to a Paint table.
uint16 	glyphID;// 	Glyph ID for the source outline.
}PaintGlyph;

typedef struct _PaintColrGlyph{
uint8 	format;// 	Set to 11.
uint16 	glyphID;// 	Glyph ID for a BaseGlyphList base glyph.
}PaintColrGlyph ;

typedef struct _PaintTransform{
uint8 	format;// 	Set to 12.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
uint32 	transformOffset:24;// 	Offset to an Affine2x3 table.
}PaintTransform ;

typedef struct _PaintVarTransform{
uint8 	format;// 	Set to 13.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
uint32 	transformOffset:24;// 	Offset to an Affine2x3 table.
}PaintVarTransform;

typedef struct _Affine2x3{
Fixed 	xx;// 	x-component of transformed x-basis vector.
Fixed 	yx;// 	y-component of transformed x-basis vector.
Fixed 	xy;// 	x-component of transformed y-basis vector.
Fixed 	yy;// 	y-component of transformed y-basis vector.
Fixed 	dx;// 	Translation in x direction.
Fixed 	dy;// 	Translation in y direction.
}Affine2x3;

typedef struct _VarAffine2x3{
Fixed 	xx;// 	x-component of transformed x-basis vector. For variation, use varIndexBase + 0.
Fixed 	yx;// 	y-component of transformed x-basis vector. For variation, use varIndexBase + 1.
Fixed 	xy;// 	x-component of transformed y-basis vector. For variation, use varIndexBase + 2.
Fixed 	yy;// 	y-component of transformed y-basis vector. For variation, use varIndexBase + 3.
Fixed 	dx;// 	Translation in x direction. For variation, use varIndexBase + 4.
Fixed 	dy;// 	Translation in y direction. For variation, use varIndexBase + 5.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}VarAffine2x3;

typedef struct _PaintTranslate{
uint8 	format;// 	Set to 14.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
FWORD 	dx;// 	Translation in x direction.
FWORD 	dy;// 	Translation in y direction.
}PaintTranslate;

typedef struct _PaintVarTranslate{
uint8 	format;// 	Set to 15.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
FWORD 	dx;// 	Translation in x direction. For variation, use varIndexBase + 0.
FWORD 	dy;// 	Translation in y direction. For variation, use varIndexBase + 1.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap
}PaintVarTranslate;

typedef struct _PaintScale{
uint8 	format;// 	Set to 16.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scaleX;// 	Scale factor in x direction.
F2DOT14 	scaleY;// 	Scale factor in y direction.
}PaintScale;

typedef struct _PaintVarScale{
uint8 	format;// 	Set to 17.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scaleX;// 	Scale factor in x direction. For variation, use varIndexBase + 0.
F2DOT14 	scaleY;// 	Scale factor in y direction. For variation, use varIndexBase + 1.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarScale;

typedef struct _PaintScaleAroundCenter{
uint8 	format;// 	Set to 18.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scaleX;// 	Scale factor in x direction.
F2DOT14 	scaleY;// 	Scale factor in y direction.
FWORD 	centerX;// 	x coordinate for the center of scaling.
FWORD 	centerY;// 	y coordinate for the center of scaling
}PaintScaleAroundCenter;

typedef struct _PaintVarScaleAroundCenter{
uint8 	format;// 	Set to 19.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scaleX;// 	Scale factor in x direction. For variation, use varIndexBase + 0.
F2DOT14 	scaleY;// 	Scale factor in y direction. For variation, use varIndexBase + 1.
FWORD 	centerX;// 	x coordinate for the center of scaling. For variation, use varIndexBase + 2.
FWORD 	centerY;// 	y coordinate for the center of scaling. For variation, use varIndexBase + 3.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarScaleAroundCenter;

typedef struct _PaintScaleUniform{
uint8 	format;// 	Set to 20.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scale;// 	Scale factor in x and y directions.
}PaintScaleUniform;

typedef struct _PaintVarScaleUniform{
uint8 	format;// 	Set to 21.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scale;// 	Scale factor in x and y directions. For variation, use varIndexBase + 0.
uint32 	varIndexBase;//
}PaintVarScaleUniform;

typedef struct _PaintScaleUniformAroundCenter{
uint8 	format;// 	Set to 22.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scale;// 	Scale factor in x and y directions.
FWORD 	centerX;// 	x coordinate for the center of scaling.
FWORD 	centerY;// 	y coordinate for the center of scaling.
}PaintScaleUniformAroundCenter;

typedef struct _PaintVarScaleUniformAroundCenter{
uint8 	format;// 	Set to 23.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	scale;// 	Scale factor in x and y directions. For variation, use varIndexBase + 0.
FWORD 	centerX;// 	x coordinate for the center of scaling. For variation, use varIndexBase + 1.
FWORD 	centerY;// 	y coordinate for the center of scaling. For variation, use varIndexBase + 2.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarScaleUniformAroundCenter;

typedef struct _PaintRotate{
uint8 	format;// 	Set to 24.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	angle;// 	Rotation angle, 180° in counter-clockwise degrees per 1.0 of value.
}PaintRotate;

typedef struct _PaintVarRotate{
uint8 	format;// 	Set to 25.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	angle;// 	Rotation angle, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 0.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarRotate;

typedef struct _PaintRotateAroundCenter{
uint8 	format;// 	Set to 26.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	angle;// 	Rotation angle, 180° in counter-clockwise degrees per 1.0 of value.
FWORD 	centerX;// 	x coordinate for the center of rotation.
FWORD 	centerY;// 	y coordinate for the center of rotation.
}PaintRotateAroundCenter;

typedef struct _PaintVarRotateAroundCenter{
uint8 	format;// 	Set to 27.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	angle;// 	Rotation angle, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 0.
FWORD 	centerX;// 	x coordinate for the center of rotation. For variation, use varIndexBase + 1.
FWORD 	centerY;// 	y coordinate for the center of rotation. For variation, use varIndexBase + 2.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarRotateAroundCenter;

typedef struct _PaintSkew{
uint8 	format;// 	Set to 28.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	xSkewAngle;// 	Angle of skew in the direction of the x-axis, 180° in counter-clockwise degrees per 1.0 of value.
F2DOT14 	ySkewAngle;// 	Angle of skew in the direction of the y-axis, 180° in counter-clockwise degrees per 1.0 of value.
}PaintSkew;

typedef struct _PaintVarSkew{
uint8 	format;// 	Set to 29.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	xSkewAngle;// 	Angle of skew in the direction of the x-axis, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 0.
F2DOT14 	ySkewAngle;// 	Angle of skew in the direction of the y-axis, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 1.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarSkew;

typedef struct _PaintSkewAroundCenter{
uint8 	format;// 	Set to 30.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	xSkewAngle;// 	Angle of skew in the direction of the x-axis, 180° in counter-clockwise degrees per 1.0 of value.
F2DOT14 	ySkewAngle;// 	Angle of skew in the direction of the y-axis, 180° in counter-clockwise degrees per 1.0 of value.
FWORD 	centerX;// 	x coordinate for the center of rotation.
FWORD 	centerY;// 	y coordinate for the center of rotation.
}PaintSkewAroundCenter;

typedef struct _PaintVarSkewAroundCenter{
uint8 	format;// 	Set to 31.
uint32 	paintOffset:24;// 	Offset to a Paint subtable.
F2DOT14 	xSkewAngle;// 	Angle of skew in the direction of the x-axis, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 0.
F2DOT14 	ySkewAngle;// 	Angle of skew in the direction of the y-axis, 180° in counter-clockwise degrees per 1.0 of value. For variation, use varIndexBase + 1.
FWORD 	centerX;// 	x coordinate for the center of rotation. For variation, use varIndexBase + 2.
FWORD 	centerY;// 	y coordinate for the center of rotation. For variation, use varIndexBase + 3.
uint32 	varIndexBase;// 	Base index into DeltaSetIndexMap.
}PaintVarSkewAroundCenter;

typedef struct _PaintComposite{
uint8 	format;// 	Set to 32.
uint32 	sourcePaintOffset:24;// 	Offset to a source Paint table.
uint8 	compositeMode;// 	A CompositeMode enumeration value.
uint32 	backdropPaintOffset:24;// 	Offset to a backdrop Paint table.
}PaintComposite;

extern const char *paint_enum[];

//CPAL — Color Palette Table

typedef struct _CPAL_version_0{
uint16 	version;// 	Table version number (=0).
uint16 	numPaletteEntries;// 	Number of palette entries in each palette.
uint16 	numPalettes;// 	Number of palettes in the table.
uint16 	numColorRecords;// 	Total number of color records, combined for all palettes.
uint32 	colorRecordsArrayOffset;// 	Offset from the beginning of CPAL table to the first ColorRecord.
uint16 	*colorRecordIndices;//[numPalettes] 	Index of each palette’s first color record in the combined color record array.
}CPAL0;

typedef struct _CPAL_version_1{
uint16 	version;// 	Table version number (=1).
uint16 	numPaletteEntries;// 	Number of palette entries in each palette.
uint16 	numPalettes;// 	Number of palettes in the table.
uint16 	numColorRecords;// 	Total number of color records, combined for all palettes.
uint32 	colorRecordsArrayOffset;// 	Offset from the beginning of CPAL table to the first ColorRecord.
uint16 	*colorRecordIndices;//[numPalettes] 	Index of each palette’s first color record in the combined color record array.
uint32 	paletteTypesArrayOffset;// 	Offset from the beginning of CPAL table to the Palette Types Array. Set to 0 if no array is provided.
uint32 	paletteLabelsArrayOffset;// 	Offset from the beginning of CPAL table to the Palette Labels Array. Set to 0 if no array is provided.
uint32 	paletteEntryLabelsArrayOffset;// 	Offset from the beginning of CPAL table to the Palette Entry Labels Array. Set to 0 if no array is provided.
}CPAL1;

typedef struct _ColorRecord{
uint8 	blue;// 	Blue value (B0).
uint8 	green;// 	Green value (B1).
uint8 	red;// 	Red value (B2).
uint8 	alpha;// 	Alpha value (B3).
}ColorRecord;

typedef ColorRecord *colorRecords; // [numColorRecords]	Color records for all palettes
typedef uint32 *paletteTypes; //[numPalettes] Array of 32-bit flag fields that describe properties of each palette. See below for details.
typedef uint16 	*paletteLabels;//[numPalettes] 	Array of 'name' table IDs (typically in the font-specific name ID range) that specify user interface strings associated with each palette. Use 0xFFFF if no name ID is provided for a palette.
typedef uint16 	*paletteEntryLabels;//[numPaletteEntries]  	Array of 'name' table IDs (typically in the font-specific name ID range) that specify user interface strings associated with each palette entry, e.g. “Outline”, “Fill”. This set of palette entry labels applies to all palettes in the font. Use 0xFFFF if no name ID is provided for a palette entry.

// 0x0001 	USABLE_WITH_LIGHT_BACKGROUND 	Bit 0: palette is appropriate to use when displaying the font on a light background such as white.
// 0x0002 	USABLE_WITH_DARK_BACKGROUND 	Bit 1: palette is appropriate to use when displaying the font on a dark background such as black.
// 0xFFFC 	Reserved 	Reserved for future use — set to 0.

//cvar — CVT Variations Table
typedef struct _TupleVariationHeader{
uint16 	variationDataSize;// 	The size in bytes of the serialized data for this tuple variation table.
uint16 	tupleIndex;// 	A packed field. The high 4 bits are flags (see below). The low 12 bits are an index into a shared tuple records array.
Tuple 	peakTuple;// 	Peak tuple record for this tuple variation table — optional, determined by flags in the tupleIndex value.
Tuple 	intermediateStartTuple;// 	Intermediate start tuple record for this tuple variation table — optional, determined by flags in the tupleIndex value.
Tuple 	intermediateEndTuple;// 	Intermediate end tuple record for this tuple variation table — optional, determined by flags in the tupleIndex value.
}TupleVariationHeader;

typedef struct _cvar_header{
uint16 	majorVersion;// 	Major version number of the CVT variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the CVT variations table — set to 0.
uint16 	tupleVariationCount;// 	A packed field. The high 4 bits are flags, and the low 12 bits are the number of tuple-variation data tables. The count can be any number between 1 and 4095.
uint16 	dataOffset;// 	Offset from the start of the 'cvar' table to the serialized data.
TupleVariationHeader 	*tupleVariationHeaders;//[tupleVariationCount] 	Array of tuple variation headers.
}cvar_header;


//cvt — Control Value Table
typedef FWORD * cvt;

//DSIG — Digital Signature Table
typedef struct _SignatureRecord{
uint32 	format;// 	Format of the signature
uint32 	length;// 	Length of signature in bytes
uint32 	signatureBlockOffset;// 	Offset to the signature block from the beginning of the table
}SignatureRecord;

typedef struct _DSIG_header{
uint32 	version;// 	Version number of the DSIG table (0x00000001)
uint16 	numSignatures;// 	Number of signatures in the table
uint16 	flags;// 	permission flags
//Bit 0: cannot be resigned
//Bits 1-7: Reserved (Set to 0)
SignatureRecord 	*signatureRecords;//[numSignatures] 	Array of signature records
}DSIG_header;

typedef  struct _Signature_Block_Format_1{
uint16 	reserved1;// 	Reserved for future use; set to zero.
uint16 	reserved2;// 	Reserved for future use; set to zero.
uint32 	signatureLength;// 	Length (in bytes) of the PKCS#7 packet in the signature field.
uint8 	*signature;//[signatureLength] 	PKCS#7 packet
}Signature_Block_Format_1 ;

//EBDT — Embedded Bitmap Data Table
typedef struct _EBDT_Header{
uint16 	majorVersion;//	Major version of the EBDT table, = 2.
uint16 	minorVersion;// 	Minor version of the EBDT table, = 0.
}EBDT_Header;

typedef struct _EbdtComponent_record{
uint16 	glyphID;// 	Component glyph ID
int8 	xOffset;// 	Position of component left
int8 	yOffset;// 	Position of component top
}EbdtComponent_record;

typedef struct _GlyphBitmapFormat1{
SmallGlyphMetrics 	smallMetrics;// 	Metrics information for the glyph
uint8 	imageData[] ;//	Byte-aligned bitmap data
}GlyphBitmapFormat1;

typedef struct _GlyphBitmapFormat2{
SmallGlyphMetrics 	smallMetrics;// 	Metrics information for the glyph
uint8 	imageData[];// 	Bit-aligned bitmap data
}GlyphBitmapFormat2;

typedef struct _GlyphBitmapFormat3{


}GlyphBitmapFormat3;

typedef struct _GlyphBitmapFormat4{


}GlyphBitmapFormat4;

typedef struct _GlyphBitmapFormat5{
uint8 	imageData[]; //	Bit-aligned bitmap data
}GlyphBitmapFormat5;

typedef struct _GlyphBitmapFormat6{
BigGlyphMetrics 	bigMetrics;//	Metrics information for the glyph
uint8 	imageData[];// 	Byte-aligned bitmap data
}GlyphBitmapFormat6;

typedef struct _GlyphBitmapFormat7{
BigGlyphMetrics 	bigMetrics;// 	Metrics information for the glyph
uint8 	imageData[];// 	Bit-aligned bitmap data
}GlyphBitmapFormat7;

typedef struct _GlyphBitmapFormat8{
SmallGlyphMetrics 	smallMetrics;// 	Metrics information for the glyph
uint8 	pad;// 	Pad to 16-bit boundary
uint16 	numComponents;// 	Number of components
EbdtComponent_record 	components[];// 	Array of EbdtComponent records
}GlyphBitmapFormat8;

typedef struct _GlyphBitmapFormat9{
BigGlyphMetrics 	bigMetrics;// 	Metrics information for the glyph
uint16 	numComponents;// 	Number of components
EbdtComponent_record 	components[];// 	Array of EbdtComponent records
}GlyphBitmapFormat9;

//EBLC — Embedded Bitmap Location Table
typedef struct _BitmapSize_record{
Offset32 	indexSubtableListOffset;// 	Offset to IndexSubtableList, from beginning of EBLC.
uint32 	indexSubtableListSize;// 	Total size in bytes of the IndexSubtableList including its array of IndexSubtables.
uint32 	numberOfIndexSubtables;// 	Number of IndexSubtables in the IndexSubtableList.
uint32 	colorRef;// 	Not used; set to 0.
SbitLineMetrics 	hori;// 	Line metrics for text rendered horizontally.
SbitLineMetrics 	vert;// 	Line metrics for text rendered vertically.
uint16 	startGlyphIndex;// 	Lowest glyph index for this size.
uint16 	endGlyphIndex;// 	Highest glyph index for this size.
uint8 	ppemX;// 	Horizontal pixels per em.
uint8 	ppemY;// 	Vertical pixels per em.
uint8 	bitDepth;// 	bit depth: 1, 2, 4, or 8.
int8 	flags;// 	Vertical or horizontal (see Bitmap Flags, below).
}BitmapSize_record;

typedef struct _EBLC_header{
uint16 	majorVersion;// 	Major version of the EBLC table, = 2.
uint16 	minorVersion;// 	Minor version of the EBLC table, = 0.
uint32 	numSizes;// 	Number of BitmapSize records.
BitmapSize_Record 	bitmapSizes[];// 	BitmapSize records array.
}EBLC_header;

typedef struct _IndexSubtableRecord{
uint16 	firstGlyphIndex;// 	First glyph ID of this range.
uint16 	lastGlyphIndex;// 	Last glyph ID of this range (inclusive).
Offset32 	indexSubtableOffset;//

}IndexSubtableRecord;

typedef struct _IndexSubtableList{
IndexSubtableRecord 	indexSubtableRecords[];// 	Array of IndexSubtableRecords[numberOfIndexSubtables].
}IndexSubtableList;

typedef struct _IndexSubHeader_record{
uint16 	indexFormat;//	Format of the IndexSubtable.
uint16 	imageFormat;// 	Format of EBDT image data.
Offset32 	imageDataOffset;// 	Offset to image data in EBDT table.
}IndexSubHeader_record;

typedef struct _IndexSubtableFormat1{
IndexSubtableRecord 	header;// 	Header info.
Offset32 	sbitOffsets[];// 	Offsets to bitmap data for each glyph in the range.
}IndexSubtableFormat1;

typedef struct _IndexSubtableFormat2{
IndexSubtableRecord 	header;// 	Header info.
uint32 	imageSize;// 	All the glyphs are of the same size.
BigGlyphMetrics 	bigMetrics;// 	All glyphs have the same metrics; glyph data may be compressed, byte-aligned, or bit-aligned.
}IndexSubtableFormat2;

typedef struct _IndexSubtableFormat3{
IndexSubtableRecord 	header;// 	Header info.
Offset16 	sbitOffsets[];// 	Offsets to bitmap data for each glyph in the range.
}IndexSubtableFormat3;

typedef  struct _GlyphIdOffsetPair{
uint16 	glyphID;// 	Glyph ID of glyph present.
Offset16 	sbitOffset;// 	Location in EBDT.
}GlyphIdOffsetPair ;

typedef struct _IndexSubtableFormat4{
IndexSubtableRecord 	header;// 	Header info.
uint32 	numGlyphs;// 	Array length.
GlyphIdOffsetPair 	glyphArray[];//[numGlyphs + 1] 	One per glyph.
}IndexSubtableFormat4;

typedef struct _IndexSubtableFormat5{
IndexSubtableRecord 	header;// 	Header info.
uint32 	imageSize;// 	All glyphs have the same data size.
BigGlyphMetrics 	bigMetrics;// 	All glyphs have the same metrics.
uint32 	numGlyphs;// 	Array length.
uint16 	glyphIdArray[];// 	One per glyph, sorted by glyph ID.
}IndexSubtableFormat5;


//EBSC — Embedded Bitmap Scaling Table
typedef struct _BitmapScale{
SbitLineMetrics 	hori;// 	line metrics
SbitLineMetrics 	vert;// 	line metrics
uint8 	ppemX;// 	target horizontal pixels per Em
uint8 	ppemY;// 	target vertical pixels per Em
uint8 	substitutePpemX;// 	use bitmaps of this size
uint8 	substitutePpemY;// 	use bitmaps of this size
}BitmapScale ;

typedef struct _EBSC_header{
uint16 	majorVersion;// 	Major version of the EBSC table, = 2.
uint16 	minorVersion;// 	Minor version of the EBSC table, = 0.
uint32 	numSizes;//
BitmapScale 	strikes[];//[numSizes] 	Array of BitmapScale records, one for each strike.
}EBSC_header;

//fpgm — Font Program
typedef struct _fpgm_Table{
uint8 	Instructions[];//. n is the number of uint8 items that fit in the size of the table.
}fpgm_Table;

//fvar — Font Variations Table
typedef struct _Font_variations_header{
uint16 	majorVersion;// 	Major version number of the font variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the font variations table — set to 0.
Offset16 	axesArrayOffset;// 	Offset in bytes from the beginning of the table to the start of the VariationAxisRecord array.
uint16 	(reserved);// 	This field is permanently reserved. Set to 2.
uint16 	axisCount;// 	The number of variation axes in the font (the number of records in the axes array).
uint16 	axisSize;// 	The size in bytes of each VariationAxisRecord — set to 20 (0x0014) for this version.
uint16 	instanceCount;// 	The number of named instances defined in the font (the number of records in the instances array).
uint16 	instanceSize;// 	The size in bytes of each InstanceRecord — set to either axisCount * sizeof(Fixed) + 4, or axisCount * sizeof(Fixed) + 6.
}Font_variations_header ;

typedef struct _VariationAxisRecord{
Tag 	axisTag ;//	Tag identifying the design variation for the axis.
Fixed 	minValue ;//	The minimum coordinate value for the axis.
Fixed 	defaultValue;// 	The default coordinate value for the axis.
Fixed 	maxValue ;//	The maximum coordinate value for the axis.
uint16 	flags;// 	Axis qualifiers — see details below.
uint16 	axisNameID;// 	The name ID for entries in the 'name' table that provide a display name for this axis.
} VariationAxisRecord;

typedef struct _UserTuple{
Fixed 	coordinates[];//[axisCount] 	Coordinate array specifying a position within the font’s variation space.
}UserTuple;

typedef struct _InstanceRecord{
uint16 	subfamilyNameID;// 	The name ID for entries in the 'name' table that provide subfamily names for this instance.
uint16 	flags;//	Reserved for future use — set to 0.
UserTuple 	*coordinates;// 	The coordinate array for this instance.
uint16 	postScriptNameID;// 	Optional. The name ID for entries in the 'name' table that provide PostScript names for this instance.
}InstanceRecord;


typedef VariationAxisRecord axes[];//[axisCount] 	The variation axis array.
typedef InstanceRecord 	instances[];//[instanceCount] 	The named instance array.


//gasp — Grid-fitting and Scan-conversion Procedure Table
typedef  struct _GaspRange{
uint16 	rangeMaxPPEM;// 	Upper limit of range, in PPEM
uint16 	rangeGaspBehavior;// 	Flags describing desired rasterizer behavior.
}GaspRange ;

typedef struct _gasp_Header{
uint16 	version;// 	Version number (0 or 1—set to 1 in new fonts)
uint16 	numRanges;// 	Number of records to follow
GaspRange 	gaspRanges[];//[numRanges] 	Sorted by ppem
}gasp_Header;

//GDEF — Glyph Definition Table
typedef struct _GDEF_Header_1{
uint16 	majorVersion;// 	Major version of the GDEF table, = 1.
uint16 	minorVersion;// 	Minor version of the GDEF table, = 0.
Offset16 	glyphClassDefOffset;// 	Offset to class definition table for glyph type, from beginning of GDEF header (may be NULL).
Offset16 	attachListOffset;// 	Offset to attachment point list table, from beginning of GDEF header (may be NULL).
Offset16 	ligCaretListOffset;// 	Offset to ligature caret list table, from beginning of GDEF header (may be NULL).
Offset16 	markAttachClassDefOffset;// 	Offset to class definition table for mark attachment type, from beginning of GDEF header (may be NULL).
}GDEF_Header_1;//, version 1.0 ;

typedef struct _GDEF_Header_1_2{
uint16 	majorVersion;// 	Major version of the GDEF table, = 1.
uint16 	minorVersion;// 	Minor version of the GDEF table, = 2.
Offset16 	glyphClassDefOffset;// 	Offset to class definition table for glyph type, from beginning of GDEF header (may be NULL).
Offset16 	attachListOffset;// 	Offset to attachment point list table, from beginning of GDEF header (may be NULL).
Offset16 	ligCaretListOffset;// 	Offset to ligature caret list table, from beginning of GDEF header (may be NULL).
Offset16 	markAttachClassDefOffset;// 	Offset to class definition table for mark attachment type, from beginning of GDEF header (may be NULL).
Offset16 	markGlyphSetsDefOffset;// 	Offset to the table of mark glyph set definitions, from beginning of GDEF header (may be NULL).
}GDEF_Header_1_2;//, version 1.2 ;

typedef struct _GDEF_Header_1_3{
uint16 	majorVersion;// 	Major version of the GDEF table, = 1.
uint16 	minorVersion;// 	Minor version of the GDEF table, = 3.
Offset16 	glyphClassDefOffset;// 	Offset to class definition table for glyph type, from beginning of GDEF header (may be NULL).
Offset16 	attachListOffset;// 	Offset to attachment point list table, from beginning of GDEF header (may be NULL).
Offset16 	ligCaretListOffset;// 	Offset to ligature caret list table, from beginning of GDEF header (may be NULL).
Offset16 	markAttachClassDefOffset;// 	Offset to class definition table for mark attachment type, from beginning of GDEF header (may be NULL).
Offset16 	markGlyphSetsDefOffset;// 	Offset to the table of mark glyph set definitions, from beginning of GDEF header (may be NULL).
Offset32 	itemVarStoreOffset;// 	Offset to the item variation store table, from beginning of GDEF header (may be NULL).
}GDEF_Header_1_3;//, version 1.3 ;

typedef  struct _AttachList{
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of AttachList table.
uint16 	glyphCount;// 	Number of glyphs with attachment points.
Offset16 	attachPointOffsets[];//[glyphCount] 	Array of offsets to AttachPoint tables, from beginning of AttachList table, in Coverage Index order.
}AttachList ;

typedef struct _AttachPoint{
uint16 	pointCount;// 	Number of attachment points on this glyph.
uint16 	pointIndices[];//[pointCount] 	Array of contour point indices, in increasing numerical order.
}AttachPoint ;

typedef struct _LigCaretList{
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of LigCaretList table.
uint16 	ligGlyphCount;// 	Number of ligature glyphs.
Offset16 	ligGlyphOffsets[];//[ligGlyphCount] 	Array of offsets to LigGlyph tables, from beginning of LigCaretList table, in Coverage index order.
}LigCaretList ;

typedef struct _LigGlyph{
uint16 	caretCount;// 	Number of caret value tables for this ligature (components - 1).
Offset16 	caretValueOffsets[];//[caretCount] 	Array of offsets to caret value tables, from beginning of LigGlyph table, in increasing coordinate order.
}LigGlyph ;

typedef struct _CaretValueFormat1{
	uint16 	format;// 	Format identifier — format = 1.
int16 	coordinate;// 	X or Y value, in design units.
}CaretValueFormat1;

typedef struct _CaretValueFormat2{
uint16 	format;// 	Format identifier — format = 2.
uint16 	caretValuePointIndex;// 	Contour point index on glyph.
}CaretValueFormat2 ;

typedef struct _CaretValueFormat3{
uint16 	format;// 	Format identifier — format = 3.
int16 	coordinate;// 	X or Y value, in design units.
Offset16 	deviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for X or Y value-from beginning of CaretValue table.
}CaretValueFormat3;

typedef struct _MarkGlyphSets{
uint16 	format;// 	Format identifier — format = 1.
uint16 	markGlyphSetCount;// 	Number of mark glyph sets defined.
Offset32 	coverageOffsets[];//[markGlyphSetCount] 	Array of offsets to mark glyph set coverage tables, from the start of the MarkGlyphSets table.
}MarkGlyphSets ;

//glyf — Glyph Data
typedef struct _Glyph_Header{
int16 	numberOfContours;// 	If the number of contours is greater than or equal to zero, this is a simple glyph. If negative, this is a composite glyph — the value -1 should be used for composite glyphs.
int16 	xMin;// 	Minimum x for coordinate data.
int16 	yMin;// 	Minimum y for coordinate data.
int16 	xMax;// 	Maximum x for coordinate data.
int16 	yMax;// 	Maximum y for coordinate data.
}Glyph_Header ;

typedef struct _Simple_Glyph{
uint16 	*endPtsOfContours;//[numberOfContours] 	Array of point indices for the last point of each contour, in increasing numeric order.
uint16 	instructionLength;// 	Total number of bytes for instructions. If instructionLength is zero, no instructions are present for this glyph, and this field is followed directly by the flags field.
uint8 	*instructions;//[instructionLength] 	Array of instruction byte code for the glyph.
uint8 	*flags;//[variable] 	Array of flag elements. See below for details regarding the number of flag array elements.
int16 	*xCoordinates;//[variable] uint8 or 	Contour point x-coordinates. See below for details regarding the number of coordinate array elements. Coordinate for the first point is relative to (0,0); others are relative to previous point.
int16 	*yCoordinates;//[variable] uint8 or 	Contour point y-coordinates. See below for details regarding the number of coordinate array elements. Coordinate for the first point is relative to (0,0); others are relative to previous point.
}Simple_Glyph;

//Component Glyph record!!! vaag moet opgebouwd worden met in de code deze is te modulair

//GPOS — Glyph Positioning Table
typedef struct _GPOS_Header_1_0{
uint16 	majorVersion;// 	Major version of the GPOS table, = 1.
uint16 	minorVersion;// 	Minor version of the GPOS table, = 0.
Offset16 	scriptListOffset;// 	Offset to ScriptList table, from beginning of GPOS table.
Offset16 	featureListOffset;// 	Offset to FeatureList table, from beginning of GPOS table.
Offset16 	lookupListOffset;// 	Offset to LookupList table, from beginning of GPOS table.
}GPOS_Header_1_0;

typedef struct _GPOS_Header_1_1{
uint16 	majorVersion;//	Major version of the GPOS table, = 1.
uint16 	minorVersion;// 	Minor version of the GPOS table, = 1.
Offset16 	scriptListOffset;// 	Offset to ScriptList table, from beginning of GPOS table.
Offset16 	featureListOffset;// 	Offset to FeatureList table, from beginning of GPOS table.
Offset16 	lookupListOffset;// 	Offset to LookupList table, from beginning of GPOS table.
Offset32 	featureVariationsOffset;// 	Offset to FeatureVariations table, from beginning of GPOS table (may be NULL).
}GPOS_Header_1_1;

typedef struct _ValueRecord{
int16 	xPlacement;// 	Horizontal adjustment for placement, in design units.
int16 	yPlacement;// 	Vertical adjustment for placement, in design units.
int16 	xAdvance;// 	Horizontal adjustment for advance, in design units — only used for horizontal layout.
int16 	yAdvance;// 	Vertical adjustment for advance, in design units — only used for vertical layout.
Offset16 	xPlaDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for horizontal placement, from beginning of the immediate parent table (SinglePos or PairPosFormat2 lookup subtable, PairSet table within a PairPosFormat1 lookup subtable) — may be NULL.
Offset16 	yPlaDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for vertical placement, from beginning of the immediate parent table (SinglePos or PairPosFormat2 lookup subtable, PairSet table within a PairPosFormat1 lookup subtable) — may be NULL.
Offset16 	xAdvDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for horizontal advance, from beginning of the immediate parent table (SinglePos or PairPosFormat2 lookup subtable, PairSet table within a PairPosFormat1 lookup subtable) — may be NULL.
Offset16 	yAdvDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for vertical advance, from beginning of the immediate parent table (SinglePos or PairPosFormat2 lookup subtable, PairSet table within a PairPosFormat1 lookup subtable) — may be NULL.
}ValueRecord;

typedef struct _SinglePosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of SinglePos subtable.
uint16 	valueFormat;// 	Defines the types of data in the ValueRecord.
ValueRecord 	valueRecord;// 	Defines positioning value(s) — applied to all glyphs in the Coverage table.
}SinglePosFormat1;

typedef struct _SinglePosFormat2{
uint16 	format;// 	Format identifier — format = 2.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of SinglePos subtable.
uint16 	valueFormat;// 	Defines the types of data in the ValueRecords.
uint16 	valueCount;// 	Number of ValueRecords — must equal glyphCount in the Coverage table.
ValueRecord 	valueRecords[];//[valueCount] 	Array of ValueRecords — positioning values applied to glyphs.
}SinglePosFormat2;

typedef struct _PairPosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of PairPos subtable.
uint16 	valueFormat1;// 	Defines the types of data in valueRecord1 — for the first glyph in the pair (may be zero).
uint16 	valueFormat2;// 	Defines the types of data in valueRecord2 — for the second glyph in the pair (may be zero).
uint16 	pairSetCount;// 	Number of PairSet tables.
Offset16 	pairSetOffsets[];//[pairSetCount] 	Array of offsets to PairSet tables. Offsets are from beginning of PairPos subtable, ordered by Coverage Index.
}PairPosFormat1;

typedef struct _PairValue{
uint16 	secondGlyph;// 	Glyph ID of second glyph in the pair (first glyph is listed in the Coverage table).
ValueRecord 	valueRecord1;// 	Positioning data for the first glyph in the pair.
ValueRecord 	valueRecord2;// 	Positioning data for the second glyph in the pair.
}PairValue;

typedef struct _PairSet{
uint16 	pairValueCount;// 	Number of PairValue records.
PairValue 	pairValueRecords[];//[pairValueCount] 	Array of PairValue records, ordered by glyph ID of the second glyph.
}PairSet;

typedef struct _Class2{
ValueRecord 	valueRecord1;//	Positioning for first glyph — empty if valueFormat1 = 0.
ValueRecord 	valueRecord2;// 	Positioning for second glyph — empty if valueFormat2 = 0.
}Class2;

typedef struct _Class1{
Class2 	class2Records[];//[class2Count] 	Array of Class2 records, ordered by classes in classDef2
}Class1;

typedef struct _PairPosFormat2{
uint16 	format;// 	Format identifier — format = 2.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of PairPos subtable.
uint16 	valueFormat1;// 	ValueRecord definition — for the first glyph of the pair (may be zero).
uint16 	valueFormat2;// 	ValueRecord definition — for the second glyph of the pair (may be zero).
Offset16 	classDef1Offset;// 	Offset to ClassDef table, from beginning of PairPos subtable — for the first glyph of the pair.
Offset16 	classDef2Offset;// 	Offset to ClassDef table, from beginning of PairPos subtable — for the second glyph of the pair.
uint16 	class1Count;// 	Number of classes in classDef1 table — includes class 0.
uint16 	class2Count;// 	Number of classes in classDef2 table — includes class 0.
Class1 	class1Records[];//[class1Count] 	Array of Class1 records, ordered by classes in classDef1.
}PairPosFormat2;

typedef struct _EntryExit{
Offset16 	entryAnchorOffset;// 	Offset to entryAnchor table, from beginning of CursivePos subtable (may be NULL).
Offset16 	exitAnchorOffset;// 	Offset to exitAnchor table, from beginning of CursivePos subtable (may be NULL).
}EntryExit;

typedef struct _CursivePosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of CursivePos subtable.
uint16 	entryExitCount;// 	Number of EntryExit records.
EntryExit 	entryExitRecords[];//[entryExitCount] 	Array of EntryExit records, in Coverage index order.
}CursivePosFormat1;

typedef struct _MarkBasePosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	markCoverageOffset;// 	Offset to markCoverage table, from beginning of MarkBasePos subtable.
Offset16 	baseCoverageOffset;// 	Offset to baseCoverage table, from beginning of MarkBasePos subtable.
uint16 	markClassCount;// 	Number of classes defined for marks.
Offset16 	markArrayOffset;// 	Offset to MarkArray table, from beginning of MarkBasePos subtable.
Offset16 	baseArrayOffset;// 	Offset to BaseArray table, from beginning of MarkBasePos subtable.
}MarkBasePosFormat1;

typedef struct _BaseRecord{
Offset16 	baseAnchorOffsets[];//[markClassCount] 	Array of offsets (one per mark class) to Anchor tables. Offsets are from beginning of BaseArray table, ordered by class (offsets may be NULL)}BaseRecord ;
}BaseRecord;

typedef struct _BaseArray{
uint16 	baseCount;// 	Number of BaseRecords.
BaseRecord 	baseRecords[];//[baseCount] 	Array of BaseRecords, in order of baseCoverage Index.
}BaseArray;

typedef struct _MarkLigPosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	markCoverageOffset;// 	Offset to markCoverage table, from beginning of MarkLigPos subtable.
Offset16 	ligatureCoverageOffset;// 	Offset to ligatureCoverage table, from beginning of MarkLigPos subtable.
uint16 	markClassCount;// 	Number of defined mark classes.
Offset16 	markArrayOffset;// 	Offset to MarkArray table, from beginning of MarkLigPos subtable.
Offset16 	ligatureArrayOffset;// 	Offset to LigatureArray table, from beginning of MarkLigPos subtable.
}MarkLigPosFormat1;

typedef struct _LigatureArray{
uint16 	ligatureCount;// 	Number of LigatureAttach table offsets.
Offset16 	ligatureAttachOffsets[];//[ligatureCount] 	Array of offsets to LigatureAttach tables. Offsets are from beginning of LigatureArray table, ordered by ligatureCoverage index.
}LigatureArray;


typedef struct _ComponentRecord{
Offset16 	ligatureAnchorOffsets[];//[markClassCount] 	Array of offsets (one per class) to Anchor tables. Offsets are from beginning of LigatureAttach table, ordered by class (offsets may be NULL).
}ComponentRecord;

typedef struct _LigatureAttach{
uint16 	componentCount;// 	Number of ComponentRecords in this ligature.
ComponentRecord 	componentRecords[];//[componentCount] 	Array of Component records, ordered in writing direction.
}LigatureAttach;


typedef struct _MarkMarkPosFormat1{
uint16 	format;// 	Format identifier — format = 1.
Offset16 	mark1CoverageOffset;// 	Offset to Combining Mark Coverage table, from beginning of MarkMarkPos subtable.
Offset16 	mark2CoverageOffset;// 	Offset to Base Mark Coverage table, from beginning of MarkMarkPos subtable.
uint16 	markClassCount;// 	Number of Combining Mark classes defined.
Offset16 	mark1ArrayOffset;// 	Offset to MarkArray table for mark1, from beginning of MarkMarkPos subtable.
Offset16 	mark2ArrayOffset;// 	Offset to Mark2Array table for mark2, from beginning of MarkMarkPos subtable.
}MarkMarkPosFormat1;

typedef struct _PosExtensionFormat1{
uint16 	format;// 	Format identifier — format = 1.
uint16 	extensionLookupType;// 	Lookup type of subtable referenced by extensionOffset (i.e. the extension subtable).
Offset32 	extensionOffset;// 	Offset to the extension subtable, of lookup type extensionLookupType, relative to the start of the PosExtensionFormat1 subtable.
}PosExtensionFormat1 ;

typedef  struct _AnchorFormat1{
uint16 	format;// 	Format identifier — format = 1.
int16 	xCoordinate;// 	Horizontal value, in design units.
int16 	yCoordinate;// 	Vertical value, in design units.
}AnchorFormat1 ;

typedef struct _AnchorFormat2{
uint16 	format;// 	Format identifier — format = 2.
int16 	xCoordinate;// 	Horizontal value, in design units.
int16 	yCoordinate;// 	Vertical value, in design units.
uint16 	anchorPoint;// 	Index to glyph contour point.
}AnchorFormat2;

typedef struct _AnchorFormat3{
uint16 	format;// 	Format identifier — format = 3.
int16 	xCoordinate;// 	Horizontal value, in design units.
int16 	yCoordinate;// 	Vertical value, in design units.
Offset16 	xDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for X coordinate, from beginning of Anchor table (may be NULL).
Offset16 	yDeviceOffset;// 	Offset to Device table (non-variable font) / VariationIndex table (variable font) for Y coordinate, from beginning of Anchor table (may be NULL).
}AnchorFormat3;

typedef struct _MarkRecord{
uint16 	markClass;// 	Class defined for the associated mark.
Offset16 	markAnchorOffset;// 	Offset to Anchor table, from beginning of MarkArray table.
}MarkRecord;

typedef struct _MarkArray{
uint16 	markCount;// 	Number of MarkRecords.
MarkRecord 	markRecords[];//[markCount] 	Array of MarkRecords, ordered by corresponding glyphs in the associated mark Coverage table.
}MarkArray;

//GSUB — Glyph Substitution Table
typedef struct _GSUB_Header_1_0{
uint16 	majorVersion;// 	Major version of the GSUB table, = 1.
uint16 	minorVersion;// 	Minor version of the GSUB table, = 0.
Offset16 	scriptListOffset;// 	Offset to ScriptList table, from beginning of GSUB table.
Offset16 	featureListOffset;// 	Offset to FeatureList table, from beginning of GSUB table.
Offset16 	lookupListOffset;// 	Offset to LookupList table, from beginning of GSUB table.
}GSUB_Header_1_0 ;

typedef struct _GSUB_Header_1_1{
uint16 	majorVersion;// 	Major version of the GSUB table, = 1.
uint16 	minorVersion;// 	Minor version of the GSUB table, = 0.
Offset16 	scriptListOffset;// 	Offset to ScriptList table, from beginning of GSUB table.
Offset16 	featureListOffset;// 	Offset to FeatureList table, from beginning of GSUB table.
Offset16 	lookupListOffset;// 	Offset to LookupList table, from beginning of GSUB table.
Offset32 	featureVariationsOffset;// 	Offset to FeatureVariations table, from beginning of the GSUB table (may be NULL).
}GSUB_Header_1_1 ;

typedef struct _SingleSubstFormat1{
uint16 	format;// 	Format identifier: format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
int16 	deltaGlyphID;// 	Add to original glyph ID to get substitute glyph ID.
}SingleSubstFormat1;

typedef struct _SingleSubstFormat2{
uint16 	format;// 	Format identifier: format = 2.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
uint16 	glyphCount;// 	Number of glyph IDs in the substituteGlyphIDs array.
uint16 	substituteGlyphIDs[];//[glyphCount] 	Array of substitute glyph IDs — ordered by Coverage index.
}SingleSubstFormat2;

typedef struct _MultipleSubstFormat1{
uint16 	format;// 	Format identifier: format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
uint16 	sequenceCount;// 	Number of Sequence table offsets in the sequenceOffsets array.
Offset16 	sequenceOffsets[];//[sequenceCount] 	Array of offsets to Sequence tables. Offsets are from beginning of substitution subtable, ordered by Coverage index.
} MultipleSubstFormat1;

typedef struct _Sequence{
	uint16 	glyphCount;// 	Number of glyph IDs in the substituteGlyphIDs array. This must always be greater than 0.
uint16 	substituteGlyphIDs[];//[glyphCount] 	String of glyph IDs to substitute.
}Sequence;

typedef struct _AlternateSubstFormat1{
	uint16 	format;// 	Format identifier: format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
uint16 	alternateSetCount;// 	Number of AlternateSet tables
Offset16 	alternateSetOffsets[];//[alternateSetCount] 	Array of offsets to AlternateSet tables. Offsets are from beginning of substitution subtable, ordered by Coverage index.
} AlternateSubstFormat1;

typedef struct _AlternateSet{
uint16 	glyphCount;// 	Number of glyph IDs in the alternateGlyphIDs array.
uint16 	alternateGlyphIDs[];//[glyphCount] 	Array of alternate glyph IDs, in arbitrary order.
}AlternateSet;

typedef struct _LigatureSubstFormat1{
uint16 	format;// 	Format identifier: format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
uint16 	ligatureSetCount;// 	Number of LigatureSet tables.
Offset16 	ligatureSetOffsets[];//[ligatureSetCount] 	Array of offsets to LigatureSet tables. Offsets are from beginning of substitution subtable, ordered by Coverage index.
}LigatureSubstFormat1;


typedef struct _LigatureSet{
uint16 	ligatureCount;// 	Number of Ligature tables.
Offset16 	ligatureOffsets[];//[LigatureCount] 	Array of offsets to Ligature tables. Offsets are from beginning of LigatureSet table, ordered by preference.
}LigatureSet;

typedef  struct _Ligature{
uint16 	ligatureGlyph;// 	Glyph ID of ligature to substitute.
uint16 	componentCount;// 	Number of components in the ligature.
uint16 	componentGlyphIDs[];//[componentCount - 1] 	Array of component glyph IDs — start with the second component, ordered in writing direction.
}Ligature ;

typedef struct _SubstExtensionFormat1{
uint16 	format;// 	Format identifier. Set to 1.
uint16 	extensionLookupType;// 	Lookup type of subtable referenced by extensionOffset (that is, the extension subtable).
Offset32 	extensionOffset;// 	Offset to the extension subtable, of lookup type extensionLookupType, relative to the start of the ExtensionSubstFormat1 subtable.
}SubstExtensionFormat1;

typedef struct _ReverseChainSingleSubstFormat1{
uint16 	format;// 	Format identifier: format = 1.
Offset16 	coverageOffset;// 	Offset to Coverage table, from beginning of substitution subtable.
uint16 	backtrackGlyphCount;// 	Number of glyphs in the backtrack sequence.
Offset16 	*backtrackCoverageOffsets;//[backtrackGlyphCount] 	Array of offsets to coverage tables in backtrack sequence, in glyph sequence order.
uint16 	lookaheadGlyphCount;// 	Number of glyphs in lookahead sequence.
Offset16 	*lookaheadCoverageOffsets;//[lookaheadGlyphCount] 	Array of offsets to coverage tables in lookahead sequence, in glyph sequence order.
uint16 	glyphCount;// 	Number of glyph IDs in the substituteGlyphIDs array.
uint16 	substituteGlyphIDs[];//[glyphCount] 	Array of substitute glyph IDs — ordered by Coverage index.
}ReverseChainSingleSubstFormat1;

//gvar — Glyph Variations Table
typedef struct gvar_header{
uint16 	majorVersion;// 	Major version number of the glyph variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the glyph variations table — set to 0.
uint16 	axisCount;// 	The number of variation axes for this font. This must be the same number as axisCount in the 'fvar' table.
uint16 	sharedTupleCount;// 	The number of shared tuple records. Shared tuple records can be referenced within glyph variation data tables for multiple glyphs, as opposed to other tuple records stored directly within a glyph variation data table.
Offset32 	sharedTuplesOffset;// 	Offset from the start of this table to the shared tuple records.
uint16 	glyphCount;// 	The number of glyphs in this font. This must match the number of glyphs stored elsewhere in the font.
uint16 	flags;// 	Bit-field that gives the format of the offset array that follows. If bit 0 is clear, the offsets are uint16; if bit 0 is set, the offsets are uint32.
Offset32 	glyphVariationDataArrayOffset;// 	Offset from the start of this table to the array of GlyphVariationData tables.
void* glyphVariationDataOffsets[];//Offset16 or Offset32[glyphCount + 1] 	Offsets from the start of the GlyphVariationData array to each GlyphVariationData table.
}gvar_header;

typedef struct _Shared_tuples{
Tuple 	sharedTuples[];//[sharedTupleCount] 	Array of tuple records shared across all glyph variation data tables.
}Shared_tuples;

typedef  struct _GlyphVariationData_header{
int16 	tupleVariationCount;// 	A packed field. The high 4 bits are flags, and the low 12 bits are the number of tuple variation tables for this glyph. The number of tuple variation tables can be any number between 1 and 4095.
Offset16 	dataOffset;// 	Offset from the start of the GlyphVariationData table to the serialized data
TupleVariationHeader 	tupleVariationHeaders[];//[tupleCount] 	Array of tuple variation headers.
}GlyphVariationData_header;

//hdmx — Horizontal Device Metrics

typedef struct _DeviceRecord{
uint8 	pixelSize;// 	Pixel size for following widths (as ppem).
uint8 	maxWidth;// 	Maximum width.
uint8 	widths[];//[numGlyphs] 	Array of widths (numGlyphs is from the 'maxp' table).
}DeviceRecord;

typedef struct _HdmxHeader{
uint16 	version;// 	Table version number—set to 0.
uint16 	numRecords;// 	Number of device records.
uint32 	sizeDeviceRecord;// 	Size of a device record, 32-bit aligned.
DeviceRecord 	records[];//[numRecords] 	Array of device records.
}HdmxHeader;

//head — Font Header Table
typedef struct _head_header{
uint16 	majorVersion;// 	Major version number of the font header table — set to 1.
uint16 	minorVersion;// 	Minor version number of the font header table — set to 0.
Fixed 	fontRevision;// 	Set by font manufacturer.
uint32 	checksumAdjustment;// 	To compute: set it to 0, sum the entire font as uint32, then store 0xB1B0AFBA - sum. If the font is used as a component in a font collection file, the value of this field will be invalidated by changes to the file structure and font table directory, and must be ignored.
uint32 	magicNumber;// 	Set to 0x5F0F3CF5.
uint16 	flags;
/* 	Bit 0: Baseline for font at y=0.

Bit 1: Left sidebearing point at x=0 (relevant only for TrueType rasterizers) — see additional information below regarding variable fonts.

Bit 2: Instructions may depend on point size.

Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear. It is strongly recommended that this be set in hinted fonts.

Bit 4: Instructions may alter advance width (the advance widths might not scale linearly).

Bit 5: This bit is not used in OpenType, and should not be set in order to ensure compatible behavior on all platforms. If set, it may result in different behavior for vertical layout in some platforms. (See Apple’s specification for details regarding behavior in Apple platforms.)

Bits 6 – 10: These bits are not used in OpenType and should always be cleared. (See Apple’s specification for details regarding legacy use in Apple platforms.)

Bit 11: Font data is “lossless” as a result of having been subjected to optimizing transformation and/or compression (such as compression mechanisms defined by ISO/IEC 14496-18, MicroType® Express, WOFF 2.0, or similar) where the original font functionality and features are retained but the binary compatibility between input and output font files is not guaranteed. As a result of the applied transform, the DSIG table may also be invalidated.

Bit 12: Font converted (produce compatible metrics).

Bit 13: Font optimized for ClearType®. Note, fonts that rely on embedded bitmaps (EBDT) for rendering should not be considered optimized for ClearType, and therefore should keep this bit cleared.

Bit 14: Last Resort font. If set, indicates that the glyphs encoded in the 'cmap' subtables are simply generic symbolic representations of code point ranges and do not truly represent support for those code points. If unset, indicates that the glyphs encoded in the 'cmap' subtables represent proper support for those code points.

Bit 15: Reserved, set to 0.*/
uint16 	unitsPerEm;/* 	Set to a value from 16 to 16384. Any value in this range is valid. In fonts that have TrueType outlines, a power of 2 is recommended as this allows performance optimization in some rasterizers.
LONGDATETIME 	created 	Number of seconds since 12:00 midnight that started January 1st, 1904, in GMT/UTC time zone.
LONGDATETIME 	modified 	Number of seconds since 12:00 midnight that started January 1st, 1904, in GMT/UTC time zone.*/
int16 	xMin;// 	Minimum x coordinate across all glyph bounding boxes.
int16 	yMin;// 	Minimum y coordinate across all glyph bounding boxes.
int16 	xMax;// 	Maximum x coordinate across all glyph bounding boxes.
int16 	yMax;// 	Maximum y coordinate across all glyph bounding boxes.
uint16 	macStyle;
/*
Bit 0: Bold (if set to 1);
Bit 1: Italic (if set to 1)
Bit 2: Underline (if set to 1)
Bit 3: Outline (if set to 1)
Bit 4: Shadow (if set to 1)
Bit 5: Condensed (if set to 1)
Bit 6: Extended (if set to 1)
Bits 7 – 15: Reserved (set to 0).
*/
uint16 	lowestRecPPEM;// 	Smallest readable size in pixels.
int16 	fontDirectionHint;//  	Deprecated (Set to 2).
/*
0: Fully mixed directional glyphs;
1: Only strongly left to right;
2: Like 1 but also contains neutrals;
-1: Only strongly right to left;
-2: Like -1 but also contains neutrals.
*/
int16 	indexToLocFormat;// 	0 for short offsets (Offset16), 1 for long (Offset32).
int16 	glyphDataFormat;// 	0 for current format.
}head_header;

//hhea — Horizontal Header Table
typedef struct _hhea_header{
uint16 	majorVersion;// 	Major version number of the horizontal header table — set to 1.
uint16 	minorVersion;// 	Minor version number of the horizontal header table — set to 0.
FWORD 	ascender;// 	Typographic ascent—see remarks below.
FWORD 	descender;// 	Typographic descent—see remarks below.
FWORD 	lineGap ;/*	Typographic line gap.
Negative lineGap values are treated as zero in some legacy platform implementations.*/
UFWORD 	advanceWidthMax; //	Maximum advance width value in 'hmtx' table.
FWORD 	minLeftSideBearing;// 	Minimum left sidebearing value in 'hmtx' table for glyphs with contours (empty glyphs should be ignored).
FWORD 	minRightSideBearing;// 	Minimum right sidebearing value; calculated as min(aw - (lsb + xMax - xMin)) for glyphs with contours (empty glyphs should be ignored).
FWORD 	xMaxExtent;// 	Max(lsb + (xMax - xMin)).
int16 	caretSlopeRise;// 	Used to calculate the slope of the cursor (rise/run); 1 for vertical.
int16 	caretSlopeRun;// 	0 for vertical.
int16 	caretOffset;// 	The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts
int16 	(reserved[4]);// 	set to 0
int16 	metricDataFormat;// 	0 for current format.
uint16 	numberOfHMetrics;// 	Number of hMetric entries in 'hmtx' table
}hhea_header;


//hmtx — Horizontal Metrics Table
typedef struct _LongHorMetric{
UFWORD 	advanceWidth;// 	Advance width, in font design units.
FWORD 	lsb ;//	Glyph left side bearing, in font design units.
}LongHorMetric;

typedef struct _Horizontal_Metrics{
LongHorMetric *	hMetrics;//[numberOfHMetrics] 	Paired advance width and left side bearing values for each glyph. Records are indexed by glyph ID.
FWORD *	leftSideBearings[];//[numGlyphs - numberOfHMetrics] 	Left side bearings for glyph IDs greater than or equal to numberOfHMetrics.
}Horizontal_Metrics;

//HVAR — Horizontal Metrics Variations Table
typedef struct _Horizontal_metrics_variations{
uint16 	majorVersion;// 	Major version number of the horizontal metrics variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the horizontal metrics variations table — set to 0.
Offset32 	itemVariationStoreOffset;// 	Offset in bytes from the start of this table to the item variation store table.
Offset32 	advanceWidthMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for advance widths (may be NULL).
Offset32 	lsbMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for left side bearings (may be NULL).
Offset32 	rsbMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for right side bearings (may be NULL).
}Horizontal_metrics_variations;

//JSTF — Justification Table

typedef struct _JstfScriptRecord{
Tag 	jstfScriptTag;// 	4-byte JstfScript identification.
Offset16 	jstfScriptOffset;// 	Offset to JstfScript table, from beginning of JSTF Header.
}JstfScriptRecord;

typedef struct _JSTF_header{
uint16 	majorVersion;// 	Major version of the JSTF table, = 1.
uint16 	minorVersion;// 	Minor version of the JSTF table, = 0.
uint16 	jstfScriptCount;// 	Number of JstfScriptRecords in this table.
JstfScriptRecord 	jstfScriptRecords[];//[jstfScriptCount] 	Array of JstfScriptRecords, in alphabetical order by jstfScriptTag.
}JSTF_header;

typedef struct _JstfLangSysRecord{
Tag 	jstfLangSysTag;// 	4-byte JstfLangSys identifier.
Offset16 	jstfLangSysOffset;// 	Offset to JstfLangSys table, from beginning of JstfScript table.
}JstfLangSysRecord;

typedef struct _JstfScript{
Offset16 	extenderGlyphOffset;// 	Offset to ExtenderGlyph table, from beginning of JstfScript table (may be NULL).
Offset16 	defJstfLangSysOffset;// 	Offset to default JstfLangSys table, from beginning of JstfScript table (may be NULL).
uint16 	jstfLangSysCount;// 	Number of JstfLangSysRecords in this table- may be zero (0).
JstfLangSysRecord 	jstfLangSysRecords[];//[jstfLangSysCount] 	Array of JstfLangSysRecords, in alphabetical order by JstfLangSysTag.
}JstfScript ;

typedef struct _ExtenderGlyph{
uint16 	glyphCountl;// 	Number of extender glyphs in this script.
uint16 	extenderGlyphs[];//[glyphCount] 	Extender glyph IDs — in increasing numerical order.
}ExtenderGlyph ;

typedef struct _JstfLangSys{
uint16 	jstfPriorityCount;// 	Number of JstfPriority tables.
Offset16 	jstfPriorityOffsets[];//[jstfPriorityCount] 	Array of offsets to JstfPriority tables, from beginning of JstfLangSys table, in priority order.
}JstfLangSys ;


typedef struct _JstfPriority{
Offset16 	gsubShrinkageEnableOffset;// 	Offset to GSUB shrinkage-enable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	gsubShrinkageDisableOffset;// 	Offset to GSUB shrinkage-disable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	gposShrinkageEnableOffset;// 	Offset to GPOS shrinkage-enable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	gposShrinkageDisableOffset;// 	Offset to GPOS shrinkage-disable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	shrinkageJstfMaxOffset;// 	Offset to shrinkage JstfMax table, from beginning of JstfPriority table (may be NULL).
Offset16 	gsubExtensionEnableOffset;// 	Offset to GSUB extension-enable JstfModList table, from beginnning of JstfPriority table (may be NULL).
Offset16 	gsubExtensionDisableOffset;// 	Offset to GSUB extension-disable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	gposExtensionEnableOffset;// 	Offset to GPOS extension-enable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	gposExtensionDisableOffset;// 	Offset to GPOS extension-disable JstfModList table, from beginning of JstfPriority table (may be NULL).
Offset16 	extensionJstfMaxOffset;// 	Offset to extension JstfMax table, from beginning of JstfPriority table (may be NULL).
}JstfPriority;

typedef struct _JstfModList{
uint16 	lookupCount;// 	Number of lookups for this modification.
uint16 	gsubLookupIndices[];//[lookupCount] 	Array of Lookup indices into the GSUB or GPOS LookupList, in increasing numerical order.
}JstfModList;

typedef struct _JstfMax{
uint16 	lookupCount;// 	Number of lookup Indices for this modification.
Offset16 	lookupOffsets[];//[lookupCount] 	Array of offsets to GPOS-type lookup tables, from beginning of JstfMax table, in design order.
}JstfMax;

//kern — Kerning
typedef struct _KernHeader{
uint16 	version;// 	Table version number—set to 0.
uint16 	nTables;// 	Number of subtables in the kerning table.
}KernHeader;

typedef struct _kerning_subtables{
uint16 	version;// 	Kern subtable version number—set to 0.
uint16 	length;// 	Length of the subtable, in bytes (including this header).
uint16 	coverage;// 	What type of information is contained in this table
}kerning_subtables;

typedef struct _KernPair{
uint16 	left;// 	The glyph index for the left-hand glyph in the kerning pair.
uint16 	right;// 	The glyph index for the right-hand glyph in the kerning pair.
FWORD 	value;// 	The kerning value for the above pair, in font design units. If this value is greater than zero, the characters will be moved apart. If this value is less than zero, the character will be moved closer together.
}KernPair;

typedef struct _KernSubtableFormat0{
uint16 	nPairs;// 	This gives the number of kerning pairs in the table.
uint16 	searchRange;// 	The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry in the table.
uint16 	entrySelector;// 	This is calculated as log2 of the largest power of two less than or equal to the value of nPairs. This value indicates how many iterations of the search loop will have to be made. (For example, in a list of eight items, there would have to be three iterations of the loop).
uint16 	rangeShift;// 	The value of nPairs minus the largest power of two less than or equal to nPairs, and then multiplied by the size in bytes of an entry in the table.
KernPair 	kernPairs[];//[nPairs] 	Array of KernPair records.
}KernSubtableFormat0;

typedef struct _KernSubtableFormat2{
uint16 	rowWidth;// 	The width, in bytes, of a row in the table.
Offset16 	leftClassOffset;// 	Offset from beginning of this subtable to left-hand class table.
Offset16 	rightClassOffset;// 	Offset from beginning of this subtable to right-hand class table.
Offset16 	kerningArrayOffset;// 	Offset from beginning of this subtable to the start of the kerning array.
}KernSubtableFormat2;

typedef struct _kern_class{
uint16 	firstGlyph;// 	First glyph in class range.
uint16 	nGlyphs;// 	Number of glyph in class range.
}kern_class;

//loca — Index to Location
typedef Offset16 Short_format[];//[numGlyphs + 1] 	The local offset divided by 2 is stored.

typedef Offset16 Long_format[];//[numGlyphs + 1] 	The actual local offset is stored.

//LTSH — Linear Threshold

typedef struct _LTSH{
uint16 	version;// 	Version number—set to 0.
uint16 	numGlyphs;// 	Number of glyphs (from numGlyphs in 'maxp' table).
uint8 	yPixels[];//[numGlyphs] 	The vertical pixel height at which the glyph can be assumed to scale linearly. On a per glyph basis.
}LTSH;

//MATH - The Mathematical Typesetting Table
typedef struct _MathValueRecord{
FWORD 	value;// 	The X or Y value in design units.
Offset16 	deviceOffset;// 	Offset to the device table, from the beginning of parent table. May be NULL. Suggested format for device table is 1.
}MathValueRecord;

typedef struct _MATH_header{
uint16 	majorVersion;//	Major version of the MATH table, = 1.
uint16 	minorVersion;// 	Minor version of the MATH table, = 0.
Offset16 	mathConstantsOffset;// 	Offset to MathConstants table, from the beginning of MATH table.
Offset16 	mathGlyphInfoOffset;// 	Offset to MathGlyphInfo table, from the beginning of MATH table.
Offset16 	mathVariantsOffset;// 	Offset to MathVariants table, from the beginning of MATH table.
}MATH_header;

typedef struct _MathConstants{
int16 	scriptPercentScaleDown;// 	Percentage of scaling down for level 1 superscripts and subscripts. Suggested value: 80%.
int16 	scriptScriptPercentScaleDown;// 	Percentage of scaling down for level 2 (scriptScript) superscripts and subscripts. Suggested value: 60%.
UFWORD 	delimitedSubFormulaMinHeight;// 	Minimum height required for a delimited expression (contained within parentheses, etc.) to be treated as a sub-formula. Suggested value: normal line height × 1.5.
UFWORD 	displayOperatorMinHeight;// 	Minimum height of n-ary operators (such as integral and summation) for formulas in display mode (that is, appearing as standalone page elements, not embedded inline within text).
MathValueRecord 	mathLeading;// 	White space to be left between math formulas to ensure proper line spacing. For example, for applications that treat line gap as a part of line ascender, formulas with ink going above (os2.sTypoAscender + os2.sTypoLineGap - MathLeading) or with ink going below os2.sTypoDescender will result in increasing line height.
MathValueRecord 	axisHeight;/* 	Axis height of the font.

In math typesetting, the term axis refers to a horizontal reference line used for positioning elements in a formula. The math axis is similar to but distinct from the baseline for regular text layout. For example, in a simple equation, a minus symbol or fraction rule would be on the axis, but a string for a variable name would be set on a baseline that is offset from the axis. The axisHeight value determines the amount of that offset.*/
MathValueRecord 	accentBaseHeight;// 	Maximum (ink) height of accent base that does not require raising the accents. Suggested: x‑height of the font (os2.sxHeight) plus any possible overshots.
MathValueRecord 	flattenedAccentBaseHeight;// 	Maximum (ink) height of accent base that does not require flattening the accents. Suggested: cap height of the font (os2.sCapHeight).
MathValueRecord 	subscriptShiftDown;// 	The standard shift down applied to subscript elements. Positive for moving in the downward direction. Suggested: os2.ySubscriptYOffset.
MathValueRecord 	subscriptTopMax;// 	Maximum allowed height of the (ink) top of subscripts that does not require moving subscripts further down. Suggested: 4/5 x- height.
MathValueRecord 	subscriptBaselineDropMin;// 	Minimum allowed drop of the baseline of subscripts relative to the (ink) bottom of the base. Checked for bases that are treated as a box or extended shape. Positive for subscript baseline dropped below the base bottom.
MathValueRecord 	superscriptShiftUp;// 	Standard shift up applied to superscript elements. Suggested: os2.ySuperscriptYOffset.
MathValueRecord 	superscriptShiftUpCramped;// 	Standard shift of superscripts relative to the base, in cramped style.
MathValueRecord 	superscriptBottomMin;// 	Minimum allowed height of the (ink) bottom of superscripts that does not require moving subscripts further up. Suggested: ¼ x-height.
MathValueRecord 	superscriptBaselineDropMax;// 	Maximum allowed drop of the baseline of superscripts relative to the (ink) top of the base. Checked for bases that are treated as a box or extended shape. Positive for superscript baseline below the base top.
MathValueRecord 	subSuperscriptGapMin;// 	Minimum gap between the superscript and subscript ink. Suggested: 4 × default rule thickness.
MathValueRecord 	superscriptBottomMaxWithSubscript;// 	The maximum level to which the (ink) bottom of superscript can be pushed to increase the gap between superscript and subscript, before subscript starts being moved down. Suggested: 4/5 x-height.
MathValueRecord 	spaceAfterScript;// 	Extra white space to be added after each subscript and superscript that occurs after a baseline element, and before each subscript and superscript that occurs before a baseline element. Suggested: 0.5 pt for a 12 pt font.

/*Note that, in some math layout implementations, a constant value, such as 0.5 pt, could be used for all text sizes. Some implementations could use a constant ratio of text size, such as 1/24 of em.*/
MathValueRecord 	upperLimitGapMin;// 	Minimum gap between the (ink) bottom of the upper limit, and the (ink) top of the base operator.
MathValueRecord 	upperLimitBaselineRiseMin;// 	Minimum distance between baseline of upper limit and (ink) top of the base operator.
MathValueRecord 	lowerLimitGapMin;// 	Minimum gap between (ink) top of the lower limit, and (ink) bottom of the base operator.
MathValueRecord 	lowerLimitBaselineDropMin;// 	Minimum distance between baseline of the lower limit and (ink) bottom of the base operator.
MathValueRecord 	stackTopShiftUp;// 	Standard shift up applied to the top element of a stack.
MathValueRecord 	stackTopDisplayStyleShiftUp;// 	Standard shift up applied to the top element of a stack in display style.
MathValueRecord 	stackBottomShiftDown;// 	Standard shift down applied to the bottom element of a stack. Positive for moving in the downward direction.
MathValueRecord 	stackBottomDisplayStyleShiftDown;// 	Standard shift down applied to the bottom element of a stack in display style. Positive for moving in the downward direction.
MathValueRecord 	stackGapMin;// 	Minimum gap between (ink) bottom of the top element of a stack, and the (ink) top of the bottom element. Suggested: 3 × default rule thickness.
MathValueRecord 	stackDisplayStyleGapMin;// 	Minimum gap between (ink) bottom of the top element of a stack, and the (ink) top of the bottom element in display style. Suggested: 7 × default rule thickness.
MathValueRecord 	stretchStackTopShiftUp;// 	Standard shift up applied to the top element of the stretch stack.
MathValueRecord 	stretchStackBottomShiftDown;// 	Standard shift down applied to the bottom element of the stretch stack. Positive for moving in the downward direction.
MathValueRecord 	stretchStackGapAboveMin;// 	Minimum gap between the ink of the stretched element, and the (ink) bottom of the element above. Suggested: same value as upperLimitGapMin.
MathValueRecord 	stretchStackGapBelowMin;// 	Minimum gap between the ink of the stretched element, and the (ink) top of the element below. Suggested: same value as lowerLimitGapMin.
MathValueRecord 	fractionNumeratorShiftUp;// 	Standard shift up applied to the numerator.
MathValueRecord 	fractionNumeratorDisplayStyleShiftUp;// 	Standard shift up applied to the numerator in display style. Suggested: same value as stackTopDisplayStyleShiftUp.
MathValueRecord 	fractionDenominatorShiftDown;// 	Standard shift down applied to the denominator. Positive for moving in the downward direction.
MathValueRecord 	fractionDenominatorDisplayStyleShiftDown;// 	Standard shift down applied to the denominator in display style. Positive for moving in the downward direction. Suggested: same value as stackBottomDisplayStyleShiftDown.
MathValueRecord 	fractionNumeratorGapMin;// 	Minimum tolerated gap between the (ink) bottom of the numerator and the ink of the fraction bar. Suggested: default rule thickness.
MathValueRecord 	fractionNumDisplayStyleGapMin;// 	Minimum tolerated gap between the (ink) bottom of the numerator and the ink of the fraction bar in display style. Suggested: 3 × default rule thickness.
MathValueRecord 	fractionRuleThickness;// 	Thickness of the fraction bar. Suggested: default rule thickness.
MathValueRecord 	fractionDenominatorGapMin;// 	Minimum tolerated gap between the (ink) top of the denominator and the ink of the fraction bar. Suggested: default rule thickness.
MathValueRecord 	fractionDenomDisplayStyleGapMin;// 	Minimum tolerated gap between the (ink) top of the denominator and the ink of the fraction bar in display style. Suggested: 3 × default rule thickness.
MathValueRecord 	skewedFractionHorizontalGap;// 	Horizontal distance between the top and bottom elements of a skewed fraction.
MathValueRecord 	skewedFractionVerticalGap;// 	Vertical distance between the ink of the top and bottom elements of a skewed fraction.
MathValueRecord 	overbarVerticalGap;// 	Distance between the overbar and the (ink) top of the base. Suggested: 3 × default rule thickness.
MathValueRecord 	overbarRuleThickness;// 	Thickness of overbar. Suggested: default rule thickness.
MathValueRecord 	overbarExtraAscender;// 	Extra white space reserved above the overbar. Suggested: default rule thickness.
MathValueRecord 	underbarVerticalGap;// 	Distance between underbar and (ink) bottom of the base. Suggested: 3 × default rule thickness.
MathValueRecord 	underbarRuleThickness;// 	Thickness of underbar. Suggested: default rule thickness.
MathValueRecord 	underbarExtraDescender;// 	Extra white space reserved below the underbar. Always positive. Suggested: default rule thickness.
MathValueRecord 	radicalVerticalGap;// 	Space between the (ink) top of the expression and the bar over it. Suggested: 1¼ default rule thickness.
MathValueRecord 	radicalDisplayStyleVerticalGap;// 	Space between the (ink) top of the expression and the bar over it. Suggested: default rule thickness + ¼ x-height.
MathValueRecord 	radicalRuleThickness;// 	Thickness of the radical rule. This is the thickness of the rule in designed or constructed radical signs. Suggested: default rule thickness.
MathValueRecord 	radicalExtraAscender;// 	Extra white space reserved above the radical. Suggested: same value as radicalRuleThickness.
MathValueRecord 	radicalKernBeforeDegree;// 	Extra horizontal kern before the degree of a radical, if such is present. Suggested: 5/18 of em.
MathValueRecord 	radicalKernAfterDegree;// 	Negative kern after the degree of a radical, if such is present. Suggested: −10/18 of em.
int16 	radicalDegreeBottomRaisePercent;// 	Height of the bottom of the radical degree, if such is present, in proportion to the height (ascender + descender) of the radical sign. Suggested: 60%.
}MathConstants;

typedef struct _MathItalicsCorrectionInfo{
Offset16 	italicsCorrectionCoverageOffset;// 	Offset to Coverage table, from the beginning of MathItalicsCorrectionInfo table.
uint16 	italicsCorrectionCount;// 	Number of italics correction values. Should coincide with the number of covered glyphs.
MathValueRecord 	italicsCorrection[];//[italicsCorrectionCount] 	Array of MathValueRecords defining italics correction values for each covered glyph.
}MathItalicsCorrectionInfo;


typedef struct _MathGlyphInfo{
Offset16 	mathItalicsCorrectionInfoOffset;// 	Offset to MathItalicsCorrectionInfo table, from the beginning of the MathGlyphInfo table.
Offset16 	mathTopAccentAttachmentOffset;// 	Offset to MathTopAccentAttachment table, from the beginning of the MathGlyphInfo table.
Offset16 	extendedShapeCoverageOffset;// 	Offset to ExtendedShapes coverage table, from the beginning of the MathGlyphInfo table. When the glyph to the left or right of a box is an extended shape variant, the (ink) box should be used for vertical positioning purposes, not the default position defined by values in MathConstants table. May be NULL.
Offset16 	mathKernInfoOffset;// 	Offset to MathKernInfo table, from the beginning of the MathGlyphInfo table.
}MathGlyphInfo;

typedef struct _MathTopAccentAttachment{
Offset16 	topAccentCoverageOffset;// 	Offset to Coverage table, from the beginning of the MathTopAccentAttachment table.
uint16 	topAccentAttachmentCount;// 	Number of top accent attachment point values. Must be the same as the number of glyph IDs referenced in the Coverage table.
MathValueRecord 	topAccentAttachment[];//[topAccentAttachmentCount] 	Array of MathValueRecords defining top accent attachment points for each covered glyph.
}MathTopAccentAttachment;

typedef struct _MathKernInfoRecord{
Offset16 	topRightMathKernOffset;//	Offset to MathKern table for top right corner, from the beginning of the MathKernInfo table. May be NULL.
Offset16 	topLeftMathKernOffset;// 	Offset to MathKern table for the top left corner, from the beginning of the MathKernInfo table. May be NULL.
Offset16 	bottomRightMathKernOffset;// 	Offset to MathKern table for bottom right corner, from the beginning of the MathKernInfo table. May be NULL.
Offset16 	bottomLeftMathKernOffset;// 	Offset to MathKern table for bottom left corner, from the beginning of the MathKernInfo table. May be NULL.
}MathKernInfoRecord;

typedef struct _MathKernInfo{
Offset16 	mathKernCoverageOffset;// 	Offset to Coverage table, from the beginning of the MathKernInfo table.
uint16 	mathKernCount;// 	Number of MathKernInfoRecords. Must be the same as the number of glyph IDs referenced in the Coverage table.
MathKernInfoRecord 	mathKernInfoRecords[];//[mathKernCount] 	Array of MathKernInfoRecords, one for each covered glyph.
}MathKernInfo;

typedef struct _MathKern{
uint16 	heightCount;// 	Number of heights at which the kern value changes.
MathValueRecord 	*correctionHeight;//[heightCount] 	Array of correction heights, in design units, sorted from lowest to highest.
MathValueRecord 	kernValues[];//[heightCount + 1] 	Array of kerning values for different height ranges. Negative values are used to move glyphs closer to each other.
}MathKern;

typedef struct _MathVariants{
UFWORD 	minConnectorOverlap;// 	Minimum overlap of connecting glyphs during glyph construction, in design units.
Offset16 	vertGlyphCoverageOffset;// 	Offset to Coverage table, from the beginning of the MathVariants table.
Offset16 	horizGlyphCoverageOffset;// 	Offset to Coverage table, from the beginning of the MathVariants table.
uint16 	vertGlyphCount;// 	Number of glyphs for which information is provided for vertically growing variants. Must be the same as the number of glyph IDs referenced in the vertical Coverage table.
uint16 	horizGlyphCount;// 	Number of glyphs for which information is provided for horizontally growing variants. Must be the same as the number of glyph IDs referenced in the horizontal Coverage table.
Offset16 	*vertGlyphConstructionOffsets;//[vertGlyphCount] 	Array of offsets to MathGlyphConstruction tables, from the beginning of the MathVariants table, for shapes growing in the vertical direction.
Offset16 	*horizGlyphConstructionOffsets;//[horizGlyphCount] 	Array of offsets to MathGlyphConstruction tables, from the beginning of the MathVariants table, for shapes growing in the horizontal direction.
}MathVariants ;


typedef struct _MathGlyphVariantRecord{
uint16 	variantGlyph;// 	Glyph ID for the variant.
UFWORD 	advanceMeasurement;// 	Advance width/height, in design units, of the variant, in the direction of requested glyph extension.
}MathGlyphVariantRecord ;

typedef struct _MathGlyphConstruction{
Offset16 	glyphAssemblyOffset;// 	Offset to the GlyphAssembly table for this shape, from the beginning of the MathGlyphConstruction table. May be NULL.
uint16 	variantCount;// 	Count of glyph growing variants for this glyph.
MathGlyphVariantRecord 	mathGlyphVariantRecords[];//[variantCount] 	MathGlyphVariantRecords for alternative variants of the glyphs.
}MathGlyphConstruction;

typedef struct _GlyphPart{
uint16 	glyphID;// 	Glyph ID for the part.
UFWORD 	startConnectorLength;// 	Advance width/ height, in design units, of the straight bar connector material at the start of the glyph in the direction of the extension (the left end for horizontal extension, the bottom end for vertical extension).
UFWORD 	endConnectorLength;// 	Advance width/ height, in design units, of the straight bar connector material at the end of the glyph in the direction of the extension (the right end for horizontal extension, the top end for vertical extension).
UFWORD 	fullAdvance ;//	Full advance width/height for this part in the direction of the extension, in design units.
uint16 	partFlags;/* 	Part qualifiers. PartFlags enumeration currently uses only one bit:
0x0001 EXTENDER_FLAG: If set, the part can be skipped or repeated.
0xFFFE Reserved.*/
}GlyphPart;

typedef struct _GlyphAssembly{
MathValueRecord 	italicsCorrection;// 	Italics correction of this GlyphAssembly. Should not depend on the assembly size.
uint16 	partCount;// 	Number of parts in this assembly.
GlyphPart 	partRecords[];//[partCount] 	Array of GlyphPart records, from left to right (for assemblies that extend horizontally) or bottom to top (for assemblies that extend vertically).
}GlyphAssembly;

//maxp — Maximum Profile

typedef struct _maxp_0_5{
Version16Dot16 	version;// 	0x00005000 for version 0.5
uint16 	numGlyphs;// 	The number of glyphs in the font.
}_maxp_0_5;


typedef struct _maxp_1_0{
Version16Dot16 	version;// 	0x00010000 for version 1.0.
uint16 	numGlyphs;// 	The number of glyphs in the font.
uint16 	maxPoints;// 	Maximum points in a non-composite glyph.
uint16 	maxContours;// 	Maximum contours in a non-composite glyph.
uint16 	maxCompositePoints;// 	Maximum points in a composite glyph.
uint16 	maxCompositeContours;// 	Maximum contours in a composite glyph.
uint16 	maxZones;// 	1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases.
uint16 	maxTwilightPoints;// 	Maximum points used in Z0.
uint16 	maxStorage;// 	Number of Storage Area locations.
uint16 	maxFunctionDefs;// 	Number of FDEFs, equal to the highest function number + 1.
uint16 	maxInstructionDefs;// 	Number of IDEFs.
uint16 	maxStackElements;// 	Maximum stack depth across Font Program ('fpgm' table), CVT Program ('prep' table) and all glyph instructions (in the 'glyf' table).
uint16 	maxSizeOfInstructions;// 	Maximum byte count for glyph instructions.
uint16 	maxComponentElements;// 	Maximum number of components referenced at “top level” for any composite glyph.
uint16 	maxComponentDepth;// 	Maximum levels of recursion; 1 for simple components.
}_maxp_1_0;

//MERG — Merge Table
typedef struct _MergHeader{
uint16 	version;// 	Version number of the merge table — set to 0.
uint16 	mergeClassCount;// 	The number of merge classes.
Offset16 	mergeDataOffset;// 	Offset to the array of merge-entry data.
uint16 	classDefCount;// 	The number of class definition tables.
Offset16 	offsetToClassDefOffsets;// 	Offset to an array of offsets to class definition tables — in bytes from the start of the MERG table.
}MergHeader;

typedef struct _MergeEntryRow{
	uint8 	mergeEntries[];//[mergeClassCount] 	Array of merge entries.
}MergeEntryRow ;

typedef struct _MergeEntry{
MergeEntryRow mergeEntryRows[];//[mergeClassCount];
}MergeEntry;

//meta — Metadata Table
typedef struct _DataMap{
Tag 	tag;// 	A tag indicating the type of metadata.
Offset32 	dataOffset;// 	Offset in bytes from the beginning of the metadata table to the data for this tag.
uint32 	dataLength;// 	Length of the data, in bytes. The data is not required to be padded to any byte boundary.
}DataMap;

typedef struct _MetaHeader{
uint32 	version;// 	Version number of the metadata table — set to 1.
uint32 	flags;// 	Flags — currently unused; set to 0.
uint32 	(reserved);// 	Not used; set to 0.
uint32 	dataMapsCount;// 	The number of data maps in the table.
DataMap 	dataMaps[];//[dataMapsCount] 	Array of data map records.
}MetaHeader;


//MVAR — Metrics Variations Table
typedef struct _ValueRecord_MVAR{
Tag 	valueTag;//	Four-byte tag identifying a font-wide measure.
uint16 	deltaSetOuterIndex;// 	A delta-set outer index — used to select an item variation data subtable within the item variation store.
uint16 	deltaSetInnerIndex;// 	A delta-set inner index — used to select a delta-set row within an item variation data subtable.
}ValueRecord_MVAR;

typedef struct _Metrics{
uint16 	majorVersion;// 	Major version number of the metrics variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the metrics variations table — set to 0.
uint16 	(reserved);// 	Not used; set to 0.
uint16 	valueRecordSize;// 	The size in bytes of each value record — must be greater than zero.
uint16 	valueRecordCount;// 	The number of value records — may be zero.
Offset16 	itemVariationStoreOffset;// 	Offset in bytes from the start of this table to the item variation store table. If valueRecordCount is zero, set to zero; if valueRecordCount is greater than zero, must be greater than zero.
ValueRecord_MVAR 	valueRecords[];//[valueRecordCount] 	Array of value records that identify target items and the associated delta-set index for each. The valueTag records must be in binary order of their valueTag field.
}Metrics ;

//name — Naming Table
typedef struct _NameRecord{
uint16 	platformID;// 	Platform ID.
uint16 	encodingID;// 	Platform-specific encoding ID.
uint16 	languageID;// 	Language ID.
uint16 	nameID;// 	Name ID.
uint16 	length;// 	String length (in bytes).
Offset16 	stringOffset;// 	String offset from start of storage area (in bytes).
}NameRecord;

typedef struct _LangTagRecord{
uint16 	length;// 	Language-tag string length (in bytes).
Offset16 	langTagOffset;// 	Language-tag string offset from start of storage area (in bytes).
}LangTagRecord;

typedef struct _Naming_0{
uint16 	version;// 	Table version number (=0).
uint16 	count;// 	Number of name records.
Offset16 	storageOffset;// 	Offset to start of string storage (from start of table).
NameRecord 	nameRecord[];//[count] 	The name records where count is the number of records.
}Naming_0;

typedef struct _Naming_1{
uint16 	version;// 	Table version number (=1).
uint16 	count;// 	Number of name records.
Offset16 	storageOffset;// 	Offset to start of string storage (from start of table).
NameRecord 	*nameRecord;//[count] 	The name records where count is the number of records.
uint16 	langTagCount;// 	Number of language-tag records.
LangTagRecord 	langTagRecord[];//[langTagCount] 	The language-tag records where langTagCount is the number of records.
}Naming_1 ;

//OS/2 — OS/2 and Windows Metrics Table
typedef struct _OS2_v0{
uint16 	version;
FWORD 	xAvgCharWidth;
uint16 	usWeightClass;
uint16 	usWidthClass;
uint16 	fsType;
FWORD 	ySubscriptXSize;
FWORD 	ySubscriptYSize;
FWORD 	ySubscriptXOffset;
FWORD 	ySubscriptYOffset;
FWORD 	ySuperscriptXSize;
FWORD 	ySuperscriptYSize;
FWORD 	ySuperscriptXOffset;
FWORD 	ySuperscriptYOffset;
FWORD 	yStrikeoutSize;
FWORD 	yStrikeoutPosition;
int16 	sFamilyClass;
uint8 	panose[10];
uint32 	ulUnicodeRange1;
uint32 	ulUnicodeRange2;
uint32 	ulUnicodeRange3;
uint32 	ulUnicodeRange4;
Tag 	achVendID;
uint16 	fsSelection;
uint16 	usFirstCharIndex;
uint16 	usLastCharIndex;
FWORD 	sTypoAscender;
FWORD 	sTypoDescender;
FWORD 	sTypoLineGap;
UFWORD 	usWinAscent;
UFWORD 	usWinDescent;
}OS2_v0;

typedef struct _OS2_v1{
uint16 	version;
FWORD 	xAvgCharWidth;
uint16 	usWeightClass;
uint16 	usWidthClass;
uint16 	fsType;
FWORD 	ySubscriptXSize;
FWORD 	ySubscriptYSize;
FWORD 	ySubscriptXOffset;
FWORD 	ySubscriptYOffset;
FWORD 	ySuperscriptXSize;
FWORD 	ySuperscriptYSize;
FWORD 	ySuperscriptXOffset;
FWORD 	ySuperscriptYOffset;
FWORD 	yStrikeoutSize;
FWORD 	yStrikeoutPosition;
int16 	sFamilyClass;
uint8 	panose[10];
uint32 	ulUnicodeRange1;
uint32 	ulUnicodeRange2;
uint32 	ulUnicodeRange3;
uint32 	ulUnicodeRange4;
Tag 	achVendID;
uint16 	fsSelection;
uint16 	usFirstCharIndex;
uint16 	usLastCharIndex;
FWORD 	sTypoAscender;
FWORD 	sTypoDescender;
FWORD 	sTypoLineGap;
UFWORD 	usWinAscent;
UFWORD 	usWinDescent;
uint32 	ulCodePageRange1;
uint32 	ulCodePageRange2;
}OS2_v1;

typedef struct _OS2_v4{
uint16 	version;
FWORD 	xAvgCharWidth;
uint16 	usWeightClass;
uint16 	usWidthClass;
uint16 	fsType;
FWORD 	ySubscriptXSize;
FWORD 	ySubscriptYSize;
FWORD 	ySubscriptXOffset;
FWORD 	ySubscriptYOffset;
FWORD 	ySuperscriptXSize;
FWORD 	ySuperscriptYSize;
FWORD 	ySuperscriptXOffset;
FWORD 	ySuperscriptYOffset;
FWORD 	yStrikeoutSize;
FWORD 	yStrikeoutPosition;
int16 	sFamilyClass;
uint8 	panose[10];
uint32 	ulUnicodeRange1;
uint32 	ulUnicodeRange2;
uint32 	ulUnicodeRange3;
uint32 	ulUnicodeRange4;
Tag 	achVendID;
uint16 	fsSelection;
uint16 	usFirstCharIndex;
uint16 	usLastCharIndex;
FWORD 	sTypoAscender;
FWORD 	sTypoDescender;
FWORD 	sTypoLineGap;
UFWORD 	usWinAscent;
UFWORD 	usWinDescent;
uint32 	ulCodePageRange1;
uint32 	ulCodePageRange2;
FWORD 	sxHeight;
FWORD 	sCapHeight;
uint16 	usDefaultChar;
uint16 	usBreakChar;
uint16 	usMaxContext;
}OS2_v4;

typedef struct _OS2_v5{
uint16 	version;// 	0x0005
FWORD 	xAvgCharWidth;
uint16 	usWeightClass;
uint16 	usWidthClass ;
uint16 	fsType 	;
FWORD 	ySubscriptXSize 	;
FWORD 	ySubscriptYSize ;
FWORD 	ySubscriptXOffset;
FWORD 	ySubscriptYOffset ;
FWORD 	ySuperscriptXSize 	;
FWORD 	ySuperscriptYSize 	;
FWORD 	ySuperscriptXOffset 	;
FWORD 	ySuperscriptYOffset 	;
FWORD 	yStrikeoutSize 	;
FWORD 	yStrikeoutPosition 	;
int16 	sFamilyClass 	;
uint8 	panose[10] 	;
uint32 	ulUnicodeRange1:31;// 	Bits 0 – 31
uint32 	ulUnicodeRange2:31;// 	Bits 32 – 63
uint32 	ulUnicodeRange3:31;//	Bits 64 – 95
uint32 	ulUnicodeRange4:31;// 	Bits 96 – 127
Tag 	achVendID 	;
uint16 	fsSelection 	;
uint16 	usFirstCharIndex ;
uint16 	usLastCharIndex 	;
FWORD 	sTypoAscender 	;
FWORD 	sTypoDescender 	;
FWORD 	sTypoLineGap 	;
UFWORD 	usWinAscent 	;
UFWORD 	usWinDescent 	;
uint32 	ulCodePageRange1:31 ;//	Bits 0 – 31
uint32 	ulCodePageRange2:31 ;//	Bits 32 – 63
FWORD 	sxHeight 	;
FWORD 	sCapHeight 	;
uint16 	usDefaultChar;
uint16 	usBreakChar 	;
uint16 	usMaxContext 	;
uint16 	usLowerOpticalPointSize ;
uint16 	usUpperOpticalPointSize;
}OS2_v5;

//PCLT — PCL 5 Table

typedef  struct _PCLT{
uint16 	majorVersion;//
uint16 	minorVersion;
uint32 	fontNumber;
uint16 	pitch;
uint16 	xHeight;
uint16 	style;
uint16 	typeFamily;
uint16 	capHeight;
uint16 	symbolSet;
int8 	typeface[16];
int8 	characterComplement[8];
int8 	fileName[6];
int8 	strokeWeight;
int8 	widthType;
uint8 	serifStyle;
uint8 	(reservedP)	;//set to 0
}PCLT;

//post — PostScript Table
typedef struct _post_header{
Version16Dot16 	version;
/*0x00010000 for version 1.0
0x00020000 for version 2.0
0x00025000 for version 2.5 (deprecated)
0x00030000 for version 3.0*/
Fixed 	italicAngle;// 	Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward).
FWORD 	underlinePosition;// 	Suggested y-coordinate of the top of the underline.
FWORD 	underlineThickness;// 	Suggested values for the underline thickness. In general, the underline thickness should match the thickness of the underscore character (U+005F LOW LINE), and should also match the strikeout thickness, which is specified in the OS/2 table.
uint32 	isFixedPitch;// 	Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced).
uint32 	minMemType42;// 	Minimum memory usage when an OpenType font is downloaded.
uint32 	maxMemType42;// 	Maximum memory usage when an OpenType font is downloaded.
uint32 	minMemType1;// 	Minimum memory usage when an OpenType font is downloaded as a Type 1 font.
uint32 	maxMemType1;// 	Maximum memory usage when an OpenType font is downloaded as a Type 1 font.
}post_header;

typedef struct _post_header_appent_ver2{
uint16 	numGlyphs;// 	Number of glyphs (this should be the same as numGlyphs in 'maxp' table).
uint16 	*glyphNameIndex;//[numGlyphs] 	Array of indices into the string data. See below for details.
uint8 	stringData;//[variable] 	Storage for the string data.
}post_header_appent_ver2;

//versie 2.5 kom je bijna niet tegen

//prep — Control Value Program
typedef struct _prep{
	uint8 table[];//[ n ] 	Set of instructions executed whenever point size or font or transformation change. n is the number of uint8 items that fit in the size of the table.
}prep;

//sbix — Standard Bitmap Graphics Table
typedef struct _sbixHeader{
uint16 	version;// 	Table version number — set to 1
uint16 	flags;/* 	Bit 0: Set to 1.
Bit 1: Draw outlines.
Bits 2 to 15: reserved (set to 0).*/
uint32 	numStrikes;// 	Number of bitmap strikes.
Offset32 	strikeOffsets[];//[numStrikes] 	Offsets from the beginning of the 'sbix' table to data for each individual bitmap strike.
}sbixHeader;

typedef struct _strikes{
uint16 	ppem;// 	The PPEM size for which this strike was designed.
uint16 	ppi;// 	The device pixel density (in PPI) for which this strike was designed. (E.g., 96 PPI, 192 PPI.)
Offset32 	glyphDataOffsets[];//[numGlyphs+1] 	Offset from the beginning of the strike data header to bitmap data for an individual glyph ID.
}strikes;

typedef struct _Glyph_data{
int16 	originOffsetX;// 	The horizontal (x-axis) position of the left edge of the bitmap graphic in relation to the glyph design space origin.
int16 	originOffsetY;// 	The vertical (y-axis) position of the bottom edge of the bitmap graphic in relation to the glyph design space origin.
Tag 	graphicType;// 	Indicates the format of the embedded graphic data: one of 'jpg ', 'png ' or 'tiff', or the special format 'dupe'.
uint8 	data[];// 	The actual embedded graphic data. The total length is inferred from sequential entries in the glyphDataOffsets array and the fixed size (8 bytes) of the preceding fields.
}Glyph_data;

//STAT — Style Attributes Table
typedef struct _Style_attributes_header{
uint16 	majorVersion;// 	Major version number of the style attributes table — set to 1.
uint16 	minorVersion;// 	Minor version number of the style attributes table — set to 2.
uint16 	designAxisSize;// 	The size in bytes of each axis record.
uint16 	designAxisCount;// 	The number of axis records. In a font with an 'fvar' table, this value must be greater than or equal to the axisCount value in the 'fvar' table. In all fonts, must be greater than zero if axisValueCount is greater than zero.
Offset32 	designAxesOffset;// 	Offset in bytes from the beginning of the STAT table to the start of the design axes array. If designAxisCount is zero, set to zero; if designAxisCount is greater than zero, must be greater than zero.
uint16 	axisValueCount;// 	The number of axis value tables.
Offset32 	offsetToAxisValueOffsets;// 	Offset in bytes from the beginning of the STAT table to the start of the design axes value offsets array. If axisValueCount is zero, set to zero; if axisValueCount is greater than zero, must be greater than zero.
uint16 	elidedFallbackNameID;// 	Name ID used as fallback when projection of names into a particular font model produces a subfamily name containing only elidable elements.
}Style_attributes_header ;

typedef struct _AxisRecord{
Tag 	axisTag;// 	A tag identifying the axis of design variation.
uint16 	axisNameID;// 	The name ID for entries in the 'name' table that provide a display string for this axis.
uint16 	axisOrdering;// 	A value that applications can use to determine primary sorting of face names, or for ordering of labels when composing family or face names.
}AxisRecord;

typedef struct _stat_followup{
AxisRecord 	*designAxes;//[designAxisCount];// 	The design-axes array.
Offset16 	axisValueOffsets[];//[axisValueCount];// 	Array of offsets to axis value tables, in bytes from the start of the axis value offsets array.
}stat_followup;

typedef struct _AxisValueFormat1{
uint16 	format;// 	Format identifier — set to 1.
uint16 	axisIndex;// 	Zero-base index into the axis record array identifying the axis of design variation to which the axis value table applies. Must be less than designAxisCount.
uint16 	flags;// 	Flags — see below for details.
uint16 	valueNameID;// 	The name ID for entries in the 'name' table that provide a display string for this attribute value.
Fixed 	value;// 	A numeric value for this attribute value.
}AxisValueFormat1;

typedef struct _AxisValueFormat2{
uint16 	format;// 	Format identifier — set to 2.
uint16 	axisIndex;// 	Zero-base index into the axis record array identifying the axis of design variation to which the axis value table applies. Must be less than designAxisCount.
uint16 	flags;// 	Flags — see below for details.
uint16 	valueNameID;// 	The name ID for entries in the 'name' table that provide a display string for this attribute value.
Fixed 	nominalValue;//	A nominal numeric value for this attribute value.
Fixed 	rangeMinValue;// 	The minimum value for a range associated with the specified name ID.
Fixed 	rangeMaxValue;// 	The maximum value for a range associated with the specified name ID.
}AxisValueFormat2;

typedef struct _AxisValueFormat3{
uint16 	format;// 	Format identifier — set to 3.
uint16 	axisIndex;// 	Zero-base index into the axis record array identifying the axis of design variation to which the axis value table applies. Must be less than designAxisCount.
uint16 	flags;// 	Flags — see below for details.
uint16 	valueNameID;// 	The name ID for entries in the 'name' table that provide a display string for this attribute value.
Fixed 	value;// 	A numeric value for this attribute value.
Fixed 	linkedValue;// 	The numeric value for a style-linked mapping from this value.
}AxisValueFormat3;

typedef struct _AxisValue{
uint16 	axisIndex;// 	Zero-base index into the axis record array identifying the axis to which this value applies. Must be less than designAxisCount.
Fixed 	value;// 	A numeric value for this attribute value.
}AxisValue;

typedef struct _AxisValueFormat4{
uint16 	format;// 	Format identifier — set to 4.
uint16 	axisCount;// 	The total number of axes contributing to this axis-values combination.
uint16 	flags;// 	Flags — see below for details.
uint16 	valueNameID;// 	The name ID for entries in the 'name' table that provide a display string for this combination of axis values.
AxisValue 	axisValues[];//[axisCount] 	Array of AxisValue records that provide the combination of axis values, one for each contributing axis.
}AxisValueFormat4;

//SVG — The SVG (Scalable Vector Graphics) Table
typedef struct _SVG_Header{
uint16 	version;// 	Table version (starting at 0). Set to 0.
Offset32 	svgDocumentListOffset;// 	Offset to the SVGDocumentList, from the start of the SVG table. Must be non-zero.
uint32 	reserved;// 	Set to 0.
}SVG_Header;

typedef struct _SVGDocumentRecord{
uint16 	startGlyphID;// 	The first glyph ID for the range covered by this record.
uint16 	endGlyphID;// 	The last glyph ID for the range covered by this record.
Offset32 	svgDocOffset;// 	Offset from the beginning of the SVGDocumentList to an SVG document. Must be non-zero.
uint32 	svgDocLength;// 	Length of the SVG document data. Must be non-zero.
}SVGDocumentRecord;

typedef struct _SVGDocumentList{
uint16 	numEntries;// 	Number of SVGDocumentRecords. Must be non-zero.
SVGDocumentRecord 	documentRecords[];//[numEntries] 	Array of SVGDocumentRecords.
}SVGDocumentList;

//VDMX — Vertical Device Metrics
typedef struct _RatioRange{
uint8 	bCharSet;// 	Character set (see below).
uint8 	xRatio;// 	Value to use for x-Ratio
uint8 	yStartRatio;// 	Starting y-Ratio value.
uint8 	yEndRatio;// 	Ending y-Ratio value.
}RatioRange;

typedef struct _VdmxHeader{
uint16 	version;// 	Version number (0 or 1).
uint16 	numRecs;// 	Number of VDMX groups present
uint16 	numRatios;// 	Number of aspect ratio groupings
RatioRange *	ratRange;//[numRatios] 	Ratio record array.
Offset16 	vdmxGroupOffsets[];//[numRatios] 	Offsets from start of this table to the VDMXGroup table for a corresponding RatioRange record.
}VdmxHeader;

typedef struct _vTable{
uint16 	yPelHeight;// 	yPelHeight to which values apply.
int16 	yMax;// 	Maximum value (in pels) for this yPelHeight.
int16 	yMin;// 	Minimum value (in pels) for this yPelHeight.
}vTable;

typedef struct _VDMXGroup{
uint16 	recs;// 	Number of height records in this group
uint8 	startsz;// 	Starting yPelHeight
uint8 	endsz;// 	Ending yPelHeight
vTable 	entry[];//[recs] 	The VDMX records
}VDMXGroup;

//vhea — Vertical Header Table
typedef struct _VheaHeader_1_0{
Version16Dot16 	version;// 	Version number of the vertical header table; 0x00010000 for version 1.0
FWORD 	ascent;// 	Distance in font design units from the centerline to the previous line’s descent.
FWORD 	descent;// 	Distance in font design units from the centerline to the next line’s ascent.
FWORD 	lineGap;// 	Reserved; set to 0
UFWORD 	advanceHeightMax;// 	The maximum advance height measurement in font design units found in the font. This value must be consistent with the entries in the vertical metrics table.
FWORD 	minTop;/*
SideBearing 	The minimum top sidebearing measurement found in the font, in font design units. This value must be consistent with the entries in the vertical metrics table.*/
FWORD 	minBottom;/*
SideBearing 	The minimum bottom sidebearing measurement found in the font, in font design units. This value must be consistent with the entries in the vertical metrics table.*/
FWORD 	yMaxExtent;// 	Defined as yMaxExtent = max(tsb + (yMax - yMin)).
int16 	caretSlopeRise;// 	The value of the caretSlopeRise field divided by the value of the caretSlopeRun Field determines the slope of the caret. A value of 0 for the rise and a value of 1 for the run specifies a horizontal caret. A value of 1 for the rise and a value of 0 for the run specifies a vertical caret. Intermediate values are desirable for fonts whose glyphs are oblique or italic. For a vertical font, a horizontal caret is best.
int16 	caretSlopeRun;// 	See the caretSlopeRise field. Value=1 for nonslanted vertical fonts.
int16 	caretOffset;// 	The amount by which the highlight on a slanted glyph needs to be shifted away from the glyph in order to produce the best appearance. Set value equal to 0 for nonslanted fonts.
int16 	reserved[4];// 	Set to 0.
int16 	metricDataFormat;// 	Set to 0.
uint16 	numOfLongVerMetrics;// 	Number;// of advance heights in the vertical metrics table.
}VheaHeader_1_0 ;

typedef struct _VheaHeader_1_1{
Version16Dot16 	version;// 	Version number of the vertical header table; 0x00011000 for version 1.1
FWORD 	vertTypoAscender;/* 	The vertical typographic ascender for this font. It is the distance in font design units from the vertical center baseline to the right edge of the design space for CJK / ideographic glyphs (or “ideographic em box”).

It is usually set to (head.unitsPerEm)/2. For example, a font with an em of 1000 design units will set this field to 500. See the Baseline tags section of the OpenType Layout Tag Registry for a description of the ideographic em-box.*/
FWORD 	vertTypoDescender;/* 	The vertical typographic descender for this font. It is the distance in font design units from the vertical center baseline to the left edge of the design space for CJK / ideographic glyphs.

It is usually set to -(head.unitsPerEm/2). For example, a font with an em of 1000 design units will set this field to -500.*/
FWORD 	vertTypoLineGap;// 	The vertical typographic gap for this font. An application can determine the recommended line spacing for single spaced vertical text for an OpenType font by the following expression: ideo embox width + vhea.vertTypoLineGap.
UFWORD 	advanceHeightMax;// 	The maximum advance height measurement in font design units found in the font. This value must be consistent with the entries in the vertical metrics table.
FWORD 	minTop;/*
SideBearing 	The minimum top sidebearing measurement found in the font, in font design units. This value must be consistent with the entries in the vertical metrics table.*/
FWORD 	minBottom;/*
SideBearing 	The minimum bottom sidebearing measurement found in the font, in font design units. This value must be consistent with the entries in the vertical metrics table.*/
FWORD 	yMaxExtent;// 	Defined as yMaxExtent = max(tsb + (yMax - yMin)).
int16 	caretSlopeRise;// 	The value of the caretSlopeRise field divided by the value of the caretSlopeRun Field determines the slope of the caret. A value of 0 for the rise and a value of 1 for the run specifies a horizontal caret. A value of 1 for the rise and a value of 0 for the run specifies a vertical caret. Intermediate values are desirable for fonts whose glyphs are oblique or italic. For a vertical font, a horizontal caret is best.
int16 	caretSlopeRun;// 	See the caretSlopeRise field. Value=1 for nonslanted vertical fonts.
int16 	caretOffset;// 	The amount by which the highlight on a slanted glyph needs to be shifted away from the glyph in order to produce the best appearance. Set value equal to 0 for nonslanted fonts.
int16 	reserved[4];// 	Set to 0.

int16 	metricDataFormat;// 	Set to 0.
uint16 	numOfLongVerMetrics;// 	Number of advance heights in the vertical metrics table.
}VheaHeader_1_1;

//vmtx — Vertical Metrics Table
typedef struct _vmtx{
UFWORD 	advanceHeight;// 	The advance height of the glyph, in font design units.
FWORD 	topSideBearing;// 	The top sidebearing of the glyph, in font design units.
}vmtx ;

//VORG — Vertical Origin Table
typedef  struct _VertOriginYMetrics{
uint16 	glyphIndex;// 	Glyph index.
int16 	vertOriginY;// 	Y coordinate, in the font’s design coordinate system, of the glyph’s vertical origin.
}VertOriginYMetrics ;

typedef struct _VerticalOrigin{
uint16 	majorVersion;// 	Major version—set to 1.
uint16 	minorVersion;// 	Minor version—set to 0.
int16 	defaultVertOriginY;// 	The y coordinate of a glyph’s vertical origin, in the font’s design coordinate system, to be used if no entry is present for the glyph in the vertOriginYMetrics array.
uint16 	numVertOriginYMetrics;// 	Number of elements in the vertOriginYMetrics array.
VertOriginYMetrics 	vertOriginYMetrics[];//[numVertOriginYMetrics] 	Array of VertOriginYMetrics records, sorted by glyph ID.
}VerticalOrigin ;

//VVAR — Vertical Metrics Variations Table
typedef struct _Vertical_metrics_variations{
uint16 	majorVersion;// 	Major version number of the vertical metrics variations table — set to 1.
uint16 	minorVersion;// 	Minor version number of the vertical metrics variations table — set to 0.
Offset32 	itemVariationStoreOffset;// 	Offset in bytes from the start of this table to the item variation store table.
Offset32 	advanceHeightMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for advance heights (may be NULL).
Offset32 	tsbMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for top side bearings (may be NULL).
Offset32 	bsbMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for bottom side bearings (may be NULL).
Offset32 	vOrgMappingOffset;// 	Offset in bytes from the start of this table to the delta-set index mapping for Y coordinates of vertical origins (may be NULL).
}Vertical_metrics_variations;


//begin de copy van de orginele h bestand
typedef struct _cmap_subTable {
	uint16 platform_id;
	uint16 platform_specifid_id;
	uint32 offset;

	uint32 format;
	uint32 length;
	uint32 language;

	uint16 num_indices;
	uint32 *glyph_index_array;
} cmap_subTable;

typedef struct _cmap_Table {
	uint16 version;
	uint16 num_subtables;

	cmap_subTable *subtables;
} cmap_Table;

typedef struct _cvt_Table {
	int16 *control_values;
	uint16 num_values;
} cvt_Table;

typedef struct _fpgm_Table2 {
	uint8 *instructions;
	uint16 num_instructions;
} fpgm_Table2;

typedef struct _TTF_Segment {
	int type;
	float *x, *y;
	int16 num_points;
} TTF_Segment;

typedef struct _TTF_Contour {
	TTF_Segment *segments;
	int16 num_segments;
} TTF_Contour;

typedef struct _TTF_Outline {
	TTF_Contour *contours;
	int16 num_contours;
	float x_min;	/* Scaled outline bounds */
	float y_min;
	float x_max;
	float y_max;

	int16 point;	/* Size outline is scaled to. < 0 if unscaled. */
} TTF_Outline;

typedef struct _TTF_Bitmap {
	int w, h;
	uint32 *data;
	uint32 c;
} TTF_Bitmap;

typedef struct _TTF_Simple_Glyph {
	uint16 *end_pts_of_contours;
	uint16 instruction_length;
	uint8 *instructions;
	uint8 *flags;
	int16 *x_coordinates;
	int16 *y_coordinates;
	int16 num_points;
} TTF_Simple_Glyph;

typedef struct _TTF_Compound_Comp {
	uint16 flags;
	uint16 glyph_index;
	int16 arg1;
	int16 arg2;
	float xscale;
	float yscale;
	float scale01;
	float scale10;
	int16 xtranslate;
	int16 ytranslate;
	uint16 point1;
	uint16 point2;
} TTF_Compound_Comp;

typedef struct _TTF_Compound_Glyph {
	TTF_Compound_Comp *comps;
	uint16 num_comps;
} TTF_Compound_Glyph;

typedef struct _TTF_Glyph {
	int16 number_of_contours;
	int16 x_min;
	int16 y_min;
	int16 x_max;
	int16 y_max;

	union {
		TTF_Simple_Glyph simple;
		TTF_Compound_Glyph compound;
	} descrip;

	uint16 instruction_length;
	uint8 *instructions;

	uint32 index;

	TTF_Outline *outline;
	TTF_Bitmap *bitmap;
} TTF_Glyph;

typedef struct _glyf_Table {
	TTF_Glyph *glyphs;
	uint16 num_glyphs; /* Copied from maxp table. */
} glyf_Table;

typedef struct _head_Table {
	uint32 version;
	uint32 font_revision;
	uint32 check_sum_adjustment;
	uint32 magic_number;
	uint16 flags;
	uint16 units_per_em;
	int64 created;
	int64 modified;
	int16 x_min;
	int16 y_min;
	int16 x_max;
	int16 y_max;
	uint16 mac_style;
	uint16 lowest_rec_ppem;
	int16 font_direction_hint;
	int16 indexo_loc_format;
	int16 glyph_data_format;
} head_Table;

typedef struct _hhea_Table {
	uint32 version;
	int16 ascent;
	int16 descent;
	int16 line_gap;
	uint16 advance_width_max;
	int16 min_left_side_bearing;
	int16 min_right_side_bearing;
	int16 x_max_extent;
	int16 caret_slope_rise;
	int16 caret_slope_run;
	int16 caret_offset;
	int16 reserved1;
	int16 reserved2;
	int16 reserved3;
	int16 reserved4;
	int16 metric_data_format;
	uint16 num_of_long_hor_metrics;
} hhea_Table;

typedef struct _hmtx_Table {
	uint16 *advance_width;
	int16 *left_side_bearing;
	int16 *non_horizontal_left_side_bearing;

	uint16 num_h_metrics;
	uint16 num_non_horizontal_metrics;
} hmtx_Table;

typedef struct _loca_Table {
	uint32 *offsets;
	uint16 num_offsets;
} loca_Table;

typedef struct _maxp_Table {
	uint32 version;
	uint16 num_glyphs;
	uint16 max_points;
	uint16 max_contours;
	uint16 max_component_points;
	uint16 max_component_contours;
	uint16 max_zones;
	uint16 maxwilight_points;
	uint16 max_storage;
	uint16 max_function_defs;
	uint16 max_instruction_defs;
	uint16 max_stack_elements;
	uint16 max_size_of_instructions;
	uint16 max_component_elements;
	uint16 max_component_depth;
} maxp_Table;

typedef struct _post_Table {
	uint32 format;
	uint32 italic_angle;
	int16 underline_position;
	int16 underlinehickness;
	uint32 is_fixed_pitch;
	uint32 min_memype_42;
	uint32 max_memype_42;
	uint32 min_memype_1;
	uint32 max_memype_1;

	uint16 num_glyphs; /* Read from post table - must match maxp */
	char **glyph_names;
} post_Table;

typedef struct __attribute__((__packed__))  _TTF_Table {
	uint32 tag;
	uint32 check_sum;
	uint32 offset;
	uint32 length;

	//uint8 status;
	// union {
	// 	cmap_Table cmap;
	// 	cvt_Table cvt;
	// 	fpgm_Table fpgm;
	// 	glyf_Table glyf;
	// 	head_Table head;
	// 	hhea_Table hhea;
	// 	hmtx_Table hmtx;
	// 	loca_Table loca;
	// 	maxp_Table maxp;
	// 	post_Table post;
	// } data;

} TTF_Table;
//__attribute__((__packed__))
typedef struct __attribute__((__packed__)) _TTF_Font {
	//int fd;

    uint32 scalerype;
	uint16 numables;
	uint16 search_range;
	uint16 entry_selector;
	uint16 range_shift;

	TTF_Table *tables;

	//int16 point;
	//uint16 dpi;
	//uint16 ppem;
	//uint16 upem;
	//uint32 raster_flags;
} TTF_Font;


//extern TTF_Table * tabels;

