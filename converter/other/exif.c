/*--------------------------------------------------------------------------
  This file contains subroutines for use by Jpegtopnm to handle the
  EXIF header.

  The code is adapted from the program Jhead by Matthaias Wandel
  December 1999 - August 2000, and contributed to the public domain.
  Bryan Henderson adapted it to Netpbm in September 2001.  Bryan
  added more of Wandel's code, from Jhead 1.9 dated December 2002 in
  January 2003.

  An EXIF header is a JFIF APP1 marker.  It is generated by some
  digital cameras and contains information about the circumstances of
  the creation of the image (camera settings, etc.).

  The EXIF header uses the TIFF format, only it contains only tag
  values and no actual image.

  Note that the image format called EXIF is simply JFIF with an EXIF
  header, i.e. a subformat of JFIF.

  See the EXIF specs at http://exif.org (2001.09.01).

--------------------------------------------------------------------------*/
#include "pm_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#if MSVCRT
    #include <sys/utime.h>
#else
    #include <utime.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#include "pm_c_util.h"
#include "pm.h"
#include "nstring.h"

#include "exif.h"

static unsigned char * LastExifRefd;
static unsigned char * DirWithThumbnailPtrs;
static double FocalplaneXRes;
bool HaveXRes;
static double FocalplaneUnits;
static int ExifImageWidth;
static int MotorolaOrder = 0;

typedef struct {
    unsigned short Tag;
    const char * Desc;
}TagTable_t;


/* Describes format descriptor */
static int BytesPerFormat[] = {0,1,1,2,4,8,1,1,2,4,8,4,8};
#define NUM_FORMATS 12

#define FMT_BYTE       1 
#define FMT_STRING     2
#define FMT_USHORT     3
#define FMT_ULONG      4
#define FMT_URATIONAL  5
#define FMT_SBYTE      6
#define FMT_UNDEFINED  7
#define FMT_SSHORT     8
#define FMT_SLONG      9
#define FMT_SRATIONAL 10
#define FMT_SINGLE    11
#define FMT_DOUBLE    12

/* Describes tag values */

#define TAG_EXIF_OFFSET       0x8769
#define TAG_INTEROP_OFFSET    0xa005

#define TAG_MAKE              0x010F
#define TAG_MODEL             0x0110

#define TAG_ORIENTATION       0x0112

#define TAG_XRESOLUTION       0x011A
#define TAG_YRESOLUTION       0x011B

#define TAG_EXPOSURETIME      0x829A
#define TAG_FNUMBER           0x829D

#define TAG_SHUTTERSPEED      0x9201
#define TAG_APERTURE          0x9202
#define TAG_MAXAPERTURE       0x9205
#define TAG_FOCALLENGTH       0x920A

#define TAG_DATETIME_ORIGINAL 0x9003
#define TAG_USERCOMMENT       0x9286

#define TAG_SUBJECT_DISTANCE  0x9206
#define TAG_FLASH             0x9209

#define TAG_FOCALPLANEXRES    0xa20E
#define TAG_FOCALPLANEUNITS   0xa210
#define TAG_EXIF_IMAGEWIDTH   0xA002
#define TAG_EXIF_IMAGELENGTH  0xA003

/* the following is added 05-jan-2001 vcs */
#define TAG_EXPOSURE_BIAS     0x9204
#define TAG_WHITEBALANCE      0x9208
#define TAG_METERING_MODE     0x9207
#define TAG_EXPOSURE_PROGRAM  0x8822
#define TAG_ISO_EQUIVALENT    0x8827
#define TAG_COMPRESSION_LEVEL 0x9102

#define TAG_THUMBNAIL_OFFSET  0x0201
#define TAG_THUMBNAIL_LENGTH  0x0202

static TagTable_t const TagTable[] = {
  {   0x100,   "ImageWidth"},
  {   0x101,   "ImageLength"},
  {   0x102,   "BitsPerSample"},
  {   0x103,   "Compression"},
  {   0x106,   "PhotometricInterpretation"},
  {   0x10A,   "FillOrder"},
  {   0x10D,   "DocumentName"},
  {   0x10E,   "ImageDescription"},
  {   0x10F,   "Make"},
  {   0x110,   "Model"},
  {   0x111,   "StripOffsets"},
  {   0x112,   "Orientation"},
  {   0x115,   "SamplesPerPixel"},
  {   0x116,   "RowsPerStrip"},
  {   0x117,   "StripByteCounts"},
  {   0x11A,   "XResolution"},
  {   0x11B,   "YResolution"},
  {   0x11C,   "PlanarConfiguration"},
  {   0x128,   "ResolutionUnit"},
  {   0x12D,   "TransferFunction"},
  {   0x131,   "Software"},
  {   0x132,   "DateTime"},
  {   0x13B,   "Artist"},
  {   0x13E,   "WhitePoint"},
  {   0x13F,   "PrimaryChromaticities"},
  {   0x156,   "TransferRange"},
  {   0x200,   "JPEGProc"},
  {   0x201,   "ThumbnailOffset"},
  {   0x202,   "ThumbnailLength"},
  {   0x211,   "YCbCrCoefficients"},
  {   0x212,   "YCbCrSubSampling"},
  {   0x213,   "YCbCrPositioning"},
  {   0x214,   "ReferenceBlackWhite"},
  {   0x828D,  "CFARepeatPatternDim"},
  {   0x828E,  "CFAPattern"},
  {   0x828F,  "BatteryLevel"},
  {   0x8298,  "Copyright"},
  {   0x829A,  "ExposureTime"},
  {   0x829D,  "FNumber"},
  {   0x83BB,  "IPTC/NAA"},
  {   0x8769,  "ExifOffset"},
  {   0x8773,  "InterColorProfile"},
  {   0x8822,  "ExposureProgram"},
  {   0x8824,  "SpectralSensitivity"},
  {   0x8825,  "GPSInfo"},
  {   0x8827,  "ISOSpeedRatings"},
  {   0x8828,  "OECF"},
  {   0x9000,  "ExifVersion"},
  {   0x9003,  "DateTimeOriginal"},
  {   0x9004,  "DateTimeDigitized"},
  {   0x9101,  "ComponentsConfiguration"},
  {   0x9102,  "CompressedBitsPerPixel"},
  {   0x9201,  "ShutterSpeedValue"},
  {   0x9202,  "ApertureValue"},
  {   0x9203,  "BrightnessValue"},
  {   0x9204,  "ExposureBiasValue"},
  {   0x9205,  "MaxApertureValue"},
  {   0x9206,  "SubjectDistance"},
  {   0x9207,  "MeteringMode"},
  {   0x9208,  "LightSource"},
  {   0x9209,  "Flash"},
  {   0x920A,  "FocalLength"},
  {   0x927C,  "MakerNote"},
  {   0x9286,  "UserComment"},
  {   0x9290,  "SubSecTime"},
  {   0x9291,  "SubSecTimeOriginal"},
  {   0x9292,  "SubSecTimeDigitized"},
  {   0xA000,  "FlashPixVersion"},
  {   0xA001,  "ColorSpace"},
  {   0xA002,  "ExifImageWidth"},
  {   0xA003,  "ExifImageLength"},
  {   0xA005,  "InteroperabilityOffset"},
  {   0xA20B,  "FlashEnergy"},                 /* 0x920B in TIFF/EP */
  {   0xA20C,  "SpatialFrequencyResponse"},  /* 0x920C    -  - */
  {   0xA20E,  "FocalPlaneXResolution"},     /* 0x920E    -  - */
  {   0xA20F,  "FocalPlaneYResolution"},      /* 0x920F    -  - */
  {   0xA210,  "FocalPlaneResolutionUnit"},  /* 0x9210    -  - */
  {   0xA214,  "SubjectLocation"},             /* 0x9214    -  - */
  {   0xA215,  "ExposureIndex"},            /* 0x9215    -  - */
  {   0xA217,  "SensingMethod"},            /* 0x9217    -  - */
  {   0xA300,  "FileSource"},
  {   0xA301,  "SceneType"},
  {      0, NULL}
} ;



/*--------------------------------------------------------------------------
   Convert a 16 bit unsigned value from file's native byte order
--------------------------------------------------------------------------*/
static int Get16u(void * Short)
{
    if (MotorolaOrder){
        return (((unsigned char *)Short)[0] << 8) | 
            ((unsigned char *)Short)[1];
    }else{
        return (((unsigned char *)Short)[1] << 8) | 
            ((unsigned char *)Short)[0];
    }
}

/*--------------------------------------------------------------------------
   Convert a 32 bit signed value from file's native byte order
--------------------------------------------------------------------------*/
static int Get32s(void * Long)
{
    if (MotorolaOrder){
        return  
            ((( char *)Long)[0] << 24) | (((unsigned char *)Long)[1] << 16) |
            (((unsigned char *)Long)[2] << 8 ) | 
            (((unsigned char *)Long)[3] << 0 );
    }else{
        return  
            ((( char *)Long)[3] << 24) | (((unsigned char *)Long)[2] << 16) |
            (((unsigned char *)Long)[1] << 8 ) | 
            (((unsigned char *)Long)[0] << 0 );
    }
}

/*--------------------------------------------------------------------------
   Convert a 32 bit unsigned value from file's native byte order
--------------------------------------------------------------------------*/
static unsigned Get32u(void * Long)
{
    return (unsigned)Get32s(Long) & 0xffffffff;
}

/*--------------------------------------------------------------------------
   Display a number as one of its many formats
--------------------------------------------------------------------------*/
static void PrintFormatNumber(FILE * const file, 
                              void * const ValuePtr, 
                              int const Format, int const ByteCount)
{
    switch(Format){
    case FMT_SBYTE:
    case FMT_BYTE:      printf("%02x\n",*(unsigned char *)ValuePtr); break;
    case FMT_USHORT:    fprintf(file, "%d\n",Get16u(ValuePtr));    break;
    case FMT_ULONG:     
    case FMT_SLONG:     fprintf(file, "%d\n",Get32s(ValuePtr));    break;
    case FMT_SSHORT:    
        fprintf(file, "%hd\n",(signed short)Get16u(ValuePtr));     break;
    case FMT_URATIONAL:
    case FMT_SRATIONAL: 
        fprintf(file, "%d/%d\n",Get32s(ValuePtr), Get32s(4+(char *)ValuePtr));
        break;
    case FMT_SINGLE:    
        fprintf(file, "%f\n",(double)*(float *)ValuePtr);          break;
    case FMT_DOUBLE:    fprintf(file, "%f\n",*(double *)ValuePtr); break;
    default: 
        fprintf(file, "Unknown format %d:", Format);
        {
            int a;
            for (a=0; a < ByteCount && a < 16; ++a)
                printf("%02x", ((unsigned char *)ValuePtr)[a]);
        }
        fprintf(file, "\n");
    }
}


/*--------------------------------------------------------------------------
   Evaluate number, be it int, rational, or float from directory.
--------------------------------------------------------------------------*/
static double ConvertAnyFormat(void * ValuePtr, int Format)
{
    double Value;
    Value = 0;

    switch(Format){
        case FMT_SBYTE:     Value = *(signed char *)ValuePtr;  break;
        case FMT_BYTE:      Value = *(unsigned char *)ValuePtr;        break;

        case FMT_USHORT:    Value = Get16u(ValuePtr);          break;
        case FMT_ULONG:     Value = Get32u(ValuePtr);          break;

        case FMT_URATIONAL:
        case FMT_SRATIONAL: 
            {
                int Num,Den;
                Num = Get32s(ValuePtr);
                Den = Get32s(4+(char *)ValuePtr);
                if (Den == 0){
                    Value = 0;
                }else{
                    Value = (double)Num/Den;
                }
                break;
            }

        case FMT_SSHORT:    Value = (signed short)Get16u(ValuePtr);  break;
        case FMT_SLONG:     Value = Get32s(ValuePtr);                break;

        /* Not sure if this is correct (never seen float used in Exif format)
         */
        case FMT_SINGLE:    Value = (double)*(float *)ValuePtr;      break;
        case FMT_DOUBLE:    Value = *(double *)ValuePtr;             break;
    }
    return Value;
}

/*--------------------------------------------------------------------------
   Process one of the nested EXIF directories.
--------------------------------------------------------------------------*/
static void 
ProcessExifDir(unsigned char *  const ExifData, 
               unsigned int     const ExifLength,
               unsigned int     const DirOffset,
               ImageInfo_t *    const ImageInfoP, 
               int              const ShowTags,
               unsigned char ** const LastExifRefdP) {

    unsigned char * const DirStart = ExifData + DirOffset;
    int de;
    int a;
    int NumDirEntries;
    unsigned ThumbnailOffset = 0;
    unsigned ThumbnailSize = 0;

    NumDirEntries = Get16u(DirStart);
    #define DIR_ENTRY_ADDR(Start, Entry) (Start+2+12*(Entry))

    {
        unsigned char * DirEnd;
        DirEnd = DIR_ENTRY_ADDR(DirStart, NumDirEntries);
        if (DirEnd+4 > (ExifData+ExifLength)){
            if (DirEnd+2 == ExifData+ExifLength || 
                DirEnd == ExifData+ExifLength){
                /* Version 1.3 of jhead would truncate a bit too much.
                   This also caught later on as well.
                */
            }else{
                /* Note: Files that had thumbnails trimmed with jhead
                   1.3 or earlier might trigger this.
                */
                pm_message("Illegal directory entry size");
                return;
            }
        }
        if (DirEnd > LastExifRefd) LastExifRefd = DirEnd;
    }

    if (ShowTags){
        pm_message("Directory with %d entries",NumDirEntries);
    }

    for (de=0;de<NumDirEntries;de++){
        int Tag, Format, Components;
        unsigned char * ValuePtr;
            /* This actually can point to a variety of things; it must
               be cast to other types when used.  But we use it as a
               byte-by-byte cursor, so we declare it as a pointer to a
               generic byte here.  
            */
        int ByteCount;
        unsigned char * DirEntry;
        DirEntry = DIR_ENTRY_ADDR(DirStart, de);

        Tag = Get16u(DirEntry);
        Format = Get16u(DirEntry+2);
        Components = Get32u(DirEntry+4);

        if ((Format-1) >= NUM_FORMATS) {
            /* (-1) catches illegal zero case as unsigned underflows
               to positive large.  
            */
            pm_message("Illegal number format %d for tag %04x", Format, Tag);
            continue;
        }
        
        ByteCount = Components * BytesPerFormat[Format];

        if (ByteCount > 4){
            unsigned OffsetVal;
            OffsetVal = Get32u(DirEntry+8);
            /* If its bigger than 4 bytes, the dir entry contains an offset.*/
            if (OffsetVal+ByteCount > ExifLength){
                /* Bogus pointer offset and / or bytecount value */
                pm_message("Illegal pointer offset value in EXIF "
                           "for tag %04x.  "
                           "Offset %d bytes %d ExifLen %d\n",
                           Tag, OffsetVal, ByteCount, ExifLength);
                continue;
            }
            ValuePtr = ExifData+OffsetVal;
        }else{
            /* 4 bytes or less and value is in the dir entry itself */
            ValuePtr = DirEntry+8;
        }

        if (*LastExifRefdP < ValuePtr+ByteCount){
            /* Keep track of last byte in the exif header that was
               actually referenced.  That way, we know where the
               discardable thumbnail data begins.
            */
            *LastExifRefdP = ValuePtr+ByteCount;
        }

        if (ShowTags){
            /* Show tag name */
            for (a=0;;a++){
                if (TagTable[a].Tag == 0){
                    fprintf(stderr, "  Unknown Tag %04x Value = ", Tag);
                    break;
                }
                if (TagTable[a].Tag == Tag){
                    fprintf(stderr, "    %s = ",TagTable[a].Desc);
                    break;
                }
            }

            /* Show tag value. */
            switch(Format){

                case FMT_UNDEFINED:
                    /* Undefined is typically an ascii string. */

                case FMT_STRING:
                    /* String arrays printed without function call
                       (different from int arrays)
                    */
                    {
                        int NoPrint = 0;
                        printf("\"");
                        for (a=0;a<ByteCount;a++){
                            if (ISPRINT((ValuePtr)[a])){
                                fprintf(stderr, "%c", (ValuePtr)[a]);
                                NoPrint = 0;
                            }else{

                                /* Avoiding indicating too many
                                   unprintable characters of proprietary
                                   bits of binary information this
                                   program may not know how to parse.  
                                */
                                if (!NoPrint){
                                    fprintf(stderr, "?");
                                    NoPrint = 1;
                                }
                            }
                        }
                        fprintf(stderr, "\"\n");
                    }
                    break;

                default:
                    /* Handle arrays of numbers later (will there ever be?)*/
                    PrintFormatNumber(stderr, ValuePtr, Format, ByteCount);
            }
        }

        /* Extract useful components of tag */
        switch(Tag){

            case TAG_MAKE:
                STRSCPY(ImageInfoP->CameraMake, (char*)ValuePtr);
                break;

            case TAG_MODEL:
                STRSCPY(ImageInfoP->CameraModel, (char*)ValuePtr);
                break;

            case TAG_XRESOLUTION:
                ImageInfoP->XResolution = 
                    ConvertAnyFormat(ValuePtr, Format);
                break;
    
            case TAG_YRESOLUTION:
                ImageInfoP->YResolution = 
                    ConvertAnyFormat(ValuePtr, Format);
                break;
    
            case TAG_DATETIME_ORIGINAL:
                STRSCPY(ImageInfoP->DateTime, (char*)ValuePtr);
                ImageInfoP->DatePointer = (char*)ValuePtr;
                break;

            case TAG_USERCOMMENT:
                /* Olympus has this padded with trailing spaces.
                   Remove these first. 
                */
                for (a=ByteCount;;){
                    a--;
                    if (((char*)ValuePtr)[a] == ' '){
                        ((char*)ValuePtr)[a] = '\0';
                    }else{
                        break;
                    }
                    if (a == 0) break;
                }

                /* Copy the comment */
                if (memcmp(ValuePtr, "ASCII",5) == 0){
                    for (a=5;a<10;a++){
                        char c;
                        c = ((char*)ValuePtr)[a];
                        if (c != '\0' && c != ' '){
                            strncpy(ImageInfoP->Comments, (char*)ValuePtr+a, 
                                    199);
                            break;
                        }
                    }
                    
                }else{
                    strncpy(ImageInfoP->Comments, (char*)ValuePtr, 199);
                }
                break;

            case TAG_FNUMBER:
                /* Simplest way of expressing aperture, so I trust it the most.
                   (overwrite previously computd value if there is one)
                   */
                ImageInfoP->ApertureFNumber = 
                    (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_APERTURE:
            case TAG_MAXAPERTURE:
                /* More relevant info always comes earlier, so only
                 use this field if we don't have appropriate aperture
                 information yet. 
                */
                if (ImageInfoP->ApertureFNumber == 0){
                    ImageInfoP->ApertureFNumber = (float)
                        exp(ConvertAnyFormat(ValuePtr, Format)*log(2)*0.5);
                }
                break;

            case TAG_FOCALLENGTH:
                /* Nice digital cameras actually save the focal length
                   as a function of how farthey are zoomed in. 
                */

                ImageInfoP->FocalLength = 
                    (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_SUBJECT_DISTANCE:
                /* Inidcates the distacne the autofocus camera is focused to.
                   Tends to be less accurate as distance increases.
                */
                ImageInfoP->Distance = 
                    (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_EXPOSURETIME:
                /* Simplest way of expressing exposure time, so I
                   trust it most.  (overwrite previously computd value
                   if there is one) 
                */
                ImageInfoP->ExposureTime = 
                    (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_SHUTTERSPEED:
                /* More complicated way of expressing exposure time,
                   so only use this value if we don't already have it
                   from somewhere else.  
                */
                if (ImageInfoP->ExposureTime == 0){
                    ImageInfoP->ExposureTime = (float)
                        (1/exp(ConvertAnyFormat(ValuePtr, Format)*log(2)));
                }
                break;

            case TAG_FLASH:
                if ((int)ConvertAnyFormat(ValuePtr, Format) & 7){
                    ImageInfoP->FlashUsed = TRUE;
                }else{
                    ImageInfoP->FlashUsed = FALSE;
                }
                break;

            case TAG_ORIENTATION:
                ImageInfoP->Orientation = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                if (ImageInfoP->Orientation < 1 || 
                    ImageInfoP->Orientation > 8){
                    pm_message("Undefined rotation value %d", 
                               ImageInfoP->Orientation);
                    ImageInfoP->Orientation = 0;
                }
                break;

            case TAG_EXIF_IMAGELENGTH:
            case TAG_EXIF_IMAGEWIDTH:
                /* Use largest of height and width to deal with images
                   that have been rotated to portrait format.  
                */
                a = (int)ConvertAnyFormat(ValuePtr, Format);
                if (ExifImageWidth < a) ExifImageWidth = a;
                break;

            case TAG_FOCALPLANEXRES:
                HaveXRes = TRUE;
                FocalplaneXRes = ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_FOCALPLANEUNITS:
                switch((int)ConvertAnyFormat(ValuePtr, Format)){
                    case 1: FocalplaneUnits = 25.4; break; /* 1 inch */
                    case 2: 
                        /* According to the information I was using, 2
                           means meters.  But looking at the Cannon
                           powershot's files, inches is the only
                           sensible value.  
                        */
                        FocalplaneUnits = 25.4;
                        break;

                    case 3: FocalplaneUnits = 10;   break;  /* 1 centimeter*/
                    case 4: FocalplaneUnits = 1;    break;  /* 1 millimeter*/
                    case 5: FocalplaneUnits = .001; break;  /* 1 micrometer*/
                }
                break;

                /* Remaining cases contributed by: Volker C. Schoech
                   (schoech@gmx.de)
                */

            case TAG_EXPOSURE_BIAS:
                ImageInfoP->ExposureBias = 
                    (float) ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_WHITEBALANCE:
                ImageInfoP->Whitebalance = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_METERING_MODE:
                ImageInfoP->MeteringMode = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_EXPOSURE_PROGRAM:
                ImageInfoP->ExposureProgram = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_ISO_EQUIVALENT:
                ImageInfoP->ISOequivalent = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                if ( ImageInfoP->ISOequivalent < 50 ) 
                    ImageInfoP->ISOequivalent *= 200;
                break;

            case TAG_COMPRESSION_LEVEL:
                ImageInfoP->CompressionLevel = 
                    (int)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_THUMBNAIL_OFFSET:
                ThumbnailOffset = (unsigned)ConvertAnyFormat(ValuePtr, Format);
                DirWithThumbnailPtrs = DirStart;
                break;

            case TAG_THUMBNAIL_LENGTH:
                ThumbnailSize = (unsigned)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_EXIF_OFFSET:
            case TAG_INTEROP_OFFSET:
                {
                    unsigned int const SubdirOffset  = Get32u(ValuePtr);
                    if (SubdirOffset >= ExifLength)
                        pm_message("Illegal exif or interop offset "
                                   "directory link.  Offset is %u, "
                                   "but Exif data is only %u bytes.",
                                   SubdirOffset, ExifLength);
                    else
                        ProcessExifDir(ExifData, ExifLength, SubdirOffset, 
                                       ImageInfoP, ShowTags, LastExifRefdP);
                    continue;
                }
        }

    }


    {
        /* In addition to linking to subdirectories via exif tags,
           there's also a potential link to another directory at the end
           of each directory.  This has got to be the result of a
           committee!  
        */
        if (DIR_ENTRY_ADDR(DirStart, NumDirEntries) + 4 <= 
            ExifData+ExifLength){
            unsigned int const SubdirOffset =
                Get32u(DirStart+2+12*NumDirEntries);
            if (SubdirOffset){
                unsigned char * const SubdirStart = ExifData + SubdirOffset;
                if (SubdirStart > ExifData+ExifLength){
                    if (SubdirStart < ExifData+ExifLength+20){
                        /* Jhead 1.3 or earlier would crop the whole directory!
                           As Jhead produces this form of format incorrectness,
                           I'll just let it pass silently.
                        */
                        if (ShowTags) 
                            printf("Thumbnail removed with "
                                   "Jhead 1.3 or earlier\n");
                    }else{
                        pm_message("Illegal subdirectory link");
                    }
                }else{
                    if (SubdirOffset <= ExifLength)
                        ProcessExifDir(ExifData, ExifLength, SubdirOffset,
                                       ImageInfoP, ShowTags, LastExifRefdP);
                }
            }
        }else{
            /* The exif header ends before the last next directory pointer. */
        }
    }

    if (ThumbnailSize && ThumbnailOffset){
        if (ThumbnailSize + ThumbnailOffset <= ExifLength){
            /* The thumbnail pointer appears to be valid.  Store it. */
            ImageInfoP->ThumbnailPointer = ExifData + ThumbnailOffset;
            ImageInfoP->ThumbnailSize = ThumbnailSize;

            if (ShowTags){
                fprintf(stderr, "Thumbnail size: %d bytes\n",ThumbnailSize);
            }
        }
    }
}



void 
process_EXIF(unsigned char * const ExifData,
             unsigned int    const length,
             ImageInfo_t *   const ImageInfoP, 
             int             const ShowTags,
             const char **   const errorP) {
/*--------------------------------------------------------------------------
  Interpret an EXIF APP1 marker

  'ExifData' is the actual Exif data; it does not include the
  "Exif" identifier and length field that often prefix Exif data.

  'length' is the length of the Exif section.
--------------------------------------------------------------------------*/
    int FirstOffset;
    unsigned char * LastExifRefd;

    *errorP = NULL;  /* initial assumption */

    if (ShowTags){
        fprintf(stderr, "Exif header %d bytes long\n",length);
    }

    if (memcmp(ExifData+0,"II",2) == 0) {
        if (ShowTags) 
            fprintf(stderr, "Exif header in Intel order\n");
        MotorolaOrder = 0;
    } else {
        if (memcmp(ExifData+0, "MM", 2) == 0) {
            if (ShowTags) 
                fprintf(stderr, "Exif header in Motorola order\n");
            MotorolaOrder = 1;
        } else {
            pm_asprintf(errorP, "Invalid alignment marker in Exif "
                        "data.  First two bytes are '%c%c' (0x%02x%02x) "
                        "instead of 'II' or 'MM'.", 
                        ExifData[0], ExifData[1], ExifData[0], ExifData[1]);
        }
    }
    if (!*errorP) {
        unsigned short const start = Get16u(ExifData + 2);
        /* Check the next value for correctness. */
        if (start != 0x002a){
            pm_asprintf(errorP, "Invalid Exif header start.  "
                        "two bytes after the alignment marker "
                        "should be 0x002a, but is 0x%04x",
                        start);
        }
    }
    if (!*errorP) {
        FirstOffset = Get32u(ExifData + 4);
        if (FirstOffset < 8 || FirstOffset > 16){
            /* I used to ensure this was set to 8 (website I used
               indicated its 8) but PENTAX Optio 230 has it set
               differently, and uses it as offset. (Sept 11 2002)
                */
            pm_message("Suspicious offset of first IFD value in Exif header");
        }
        
        ImageInfoP->Comments[0] = '\0';  /* Initial value - null string */
        
        HaveXRes = FALSE;  /* Initial assumption */
        FocalplaneUnits = 0;
        ExifImageWidth = 0;
        
        LastExifRefd = ExifData;
        DirWithThumbnailPtrs = NULL;
        
        ProcessExifDir(ExifData, length, FirstOffset, 
                       ImageInfoP, ShowTags, &LastExifRefd);
        
        /* Compute the CCD width, in millimeters. */
        if (HaveXRes){
            ImageInfoP->HaveCCDWidth = 1;
            ImageInfoP->CCDWidth = 
                    (float)(ExifImageWidth * FocalplaneUnits / FocalplaneXRes);
        } else
            ImageInfoP->HaveCCDWidth = 0;
            
        if (ShowTags){
            fprintf(stderr, 
                    "Non-settings part of Exif header: %lu bytes\n",
                    (unsigned long)(ExifData+length-LastExifRefd));
        }
    }
}

/*--------------------------------------------------------------------------
   Show the collected image info, displaying camera F-stop and shutter
   speed in a consistent and legible fashion.
--------------------------------------------------------------------------*/
void 
ShowImageInfo(ImageInfo_t * const ImageInfoP)
{
    if (ImageInfoP->CameraMake[0]){
        fprintf(stderr, "Camera make  : %s\n",ImageInfoP->CameraMake);
        fprintf(stderr, "Camera model : %s\n",ImageInfoP->CameraModel);
    }
    if (ImageInfoP->DateTime[0]){
        fprintf(stderr, "Date/Time    : %s\n",ImageInfoP->DateTime);
    }
    fprintf(stderr, "Resolution   : %f x %f\n",
            ImageInfoP->XResolution, ImageInfoP->YResolution);
    if (ImageInfoP->Orientation > 1){

        /* Only print orientation if one was supplied, and if its not
           1 (normal orientation)

           1 - The 0th row is at the visual top of the image
               and the 0th column is the visual left-hand side.
           2 - The 0th row is at the visual top of the image
               and the 0th column is the visual right-hand side.
           3 - The 0th row is at the visual bottom of the image
               and the 0th column is the visual right-hand side.
           4 - The 0th row is at the visual bottom of the image
               and the 0th column is the visual left-hand side.
           5 - The 0th row is the visual left-hand side of of the image
               and the 0th column is the visual top.
           6 - The 0th row is the visual right-hand side of of the image
               and the 0th column is the visual top.
           7 - The 0th row is the visual right-hand side of of the image
               and the 0th column is the visual bottom.
           8 - The 0th row is the visual left-hand side of of the image
               and the 0th column is the visual bottom.

           Note: The descriptions here are the same as the name of the
           command line option to pass to jpegtran to right the image
        */
        static const char * OrientTab[9] = {
            "Undefined",
            "Normal",           /* 1 */
            "flip horizontal",  /* left right reversed mirror */
            "rotate 180",       /* 3 */
            "flip vertical",    /* upside down mirror */
            "transpose",    /* Flipped about top-left <--> bottom-right axis.*/
            "rotate 90",        /* rotate 90 cw to right it. */
            "transverse",   /* flipped about top-right <--> bottom-left axis */
            "rotate 270",       /* rotate 270 to right it. */
        };

        fprintf(stderr, "Orientation  : %s\n", 
                OrientTab[ImageInfoP->Orientation]);
    }

    if (ImageInfoP->IsColor == 0){
        fprintf(stderr, "Color/bw     : Black and white\n");
    }
    if (ImageInfoP->FlashUsed >= 0){
        fprintf(stderr, "Flash used   : %s\n",
                ImageInfoP->FlashUsed ? "Yes" :"No");
    }
    if (ImageInfoP->FocalLength){
        fprintf(stderr, "Focal length : %4.1fmm",
                (double)ImageInfoP->FocalLength);
        if (ImageInfoP->HaveCCDWidth){
            fprintf(stderr, "  (35mm equivalent: %dmm)",
                    (int)
                    (ImageInfoP->FocalLength/ImageInfoP->CCDWidth*36 + 0.5));
        }
        fprintf(stderr, "\n");
    }

    if (ImageInfoP->HaveCCDWidth){
        fprintf(stderr, "CCD width    : %2.4fmm\n",
                (double)ImageInfoP->CCDWidth);
    }

    if (ImageInfoP->ExposureTime){ 
        if (ImageInfoP->ExposureTime < 0.010){
            fprintf(stderr, 
                    "Exposure time: %6.4f s ",
                    (double)ImageInfoP->ExposureTime);
        }else{
            fprintf(stderr, 
                    "Exposure time: %5.3f s ",
                    (double)ImageInfoP->ExposureTime);
        }
        if (ImageInfoP->ExposureTime <= 0.5){
            fprintf(stderr, " (1/%d)",(int)(0.5 + 1/ImageInfoP->ExposureTime));
        }
        fprintf(stderr, "\n");
    }
    if (ImageInfoP->ApertureFNumber){
        fprintf(stderr, "Aperture     : f/%3.1f\n",
                (double)ImageInfoP->ApertureFNumber);
    }
    if (ImageInfoP->Distance){
        if (ImageInfoP->Distance < 0){
            fprintf(stderr, "Focus dist.  : Infinite\n");
        }else{
            fprintf(stderr, "Focus dist.  :%5.2fm\n",
                    (double)ImageInfoP->Distance);
        }
    }





    if (ImageInfoP->ISOequivalent){ /* 05-jan-2001 vcs */
        fprintf(stderr, "ISO equiv.   : %2d\n",(int)ImageInfoP->ISOequivalent);
    }
    if (ImageInfoP->ExposureBias){ /* 05-jan-2001 vcs */
        fprintf(stderr, "Exposure bias:%4.2f\n",
                (double)ImageInfoP->ExposureBias);
    }
        
    if (ImageInfoP->Whitebalance){ /* 05-jan-2001 vcs */
        switch(ImageInfoP->Whitebalance) {
        case 1:
            fprintf(stderr, "Whitebalance : sunny\n");
            break;
        case 2:
            fprintf(stderr, "Whitebalance : fluorescent\n");
            break;
        case 3:
            fprintf(stderr, "Whitebalance : incandescent\n");
            break;
        default:
            fprintf(stderr, "Whitebalance : cloudy\n");
        }
    }
    if (ImageInfoP->MeteringMode){ /* 05-jan-2001 vcs */
        switch(ImageInfoP->MeteringMode) {
        case 2:
            fprintf(stderr, "Metering Mode: center weight\n");
            break;
        case 3:
            fprintf(stderr, "Metering Mode: spot\n");
            break;
        case 5:
            fprintf(stderr, "Metering Mode: matrix\n");
            break;
        }
    }
    if (ImageInfoP->ExposureProgram){ /* 05-jan-2001 vcs */
        switch(ImageInfoP->ExposureProgram) {
        case 2:
            fprintf(stderr, "Exposure     : program (auto)\n");
            break;
        case 3:
            fprintf(stderr, "Exposure     : aperture priority (semi-auto)\n");
            break;
        case 4:
            fprintf(stderr, "Exposure     : shutter priority (semi-auto)\n");
            break;
        }
    }
    if (ImageInfoP->CompressionLevel){ /* 05-jan-2001 vcs */
        switch(ImageInfoP->CompressionLevel) {
        case 1:
            fprintf(stderr, "Jpeg Quality  : basic\n");
            break;
        case 2:
            fprintf(stderr, "Jpeg Quality  : normal\n");
            break;
        case 4:
            fprintf(stderr, "Jpeg Quality  : fine\n");
            break;
       }
    }

         

    /* Print the comment. Print 'Comment:' for each new line of comment. */
    if (ImageInfoP->Comments[0]){
        int a,c;
        fprintf(stderr, "Comment      : ");
        for (a=0;a<MAX_COMMENT;a++){
            c = ImageInfoP->Comments[a];
            if (c == '\0') break;
            if (c == '\n'){
                /* Do not start a new line if the string ends with a cr */
                if (ImageInfoP->Comments[a+1] != '\0'){
                    fprintf(stderr, "\nComment      : ");
                }else{
                    fprintf(stderr, "\n");
                }
            }else{
                putc(c, stderr);
            }
        }
        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
}




