/*===========================================================================*
 * rate.h                                                                    *
 *                                                                           *
 *      Procedures concerned with rate control
 *                                                                           *
 * EXPORTED PROCEDURES:                                                      *
 *  getRateMode()
 *  setBitRate()
 *  getBitRate()
 *  setBufferSize()
 *  getBufferSize()
 *      initRateControl()
 *      targetRateControl()
 *      updateRateControl()
 *      MB_RateOut()
 *                                                                           *
 *===========================================================================*/

/*      COPYRIGHT INFO HERE     */

#define VARIABLE_RATE 0
#define FIXED_RATE 1


/*==================*
 * Exported VARIABLES *
 *==================*/


extern int rc_bitsThisMB;
extern int rc_numBlocks;
extern int rc_totalQuant;
extern int rc_quantOverride;


/*=====================*
 * EXPORTED PROCEDURES *
 *=====================*/

/*===========================================================================*
 *
 * initRateControl
 *
 *      initialize the allocation parameters.
 *===========================================================================*/
int
initRateControl(bool const wantUnderflowWarning,
                bool const wantOverflowWarning);

/*===========================================================================*
 *
 * targetRateControl
 *
 *      Determine the target allocation for given picture type.
 *
 * RETURNS:     target size in bits
 *===========================================================================*/
void
targetRateControl(MpegFrame * const frameP);


/*===========================================================================*
 *
 * MB_RateOut
 *
 *      Prints out sampling of MB rate control data.  Every "nth" block
 *      stats are printed, with "n" controlled by global RC_MB_SAMPLE_RATE
 *
 * RETURNS:     nothing
 *===========================================================================*/
extern void MB_RateOut (int type);


/*===========================================================================*
 *
 * updateRateControl
 *
 *      Update the statistics kept, after end of frame
 *
 * RETURNS:     nothing
 *
 * SIDE EFFECTS:   many global variables
 *===========================================================================*/
void
updateRateControl(int const type);


/*===========================================================================*
 *
 * needQScaleChange(current Q scale, 4 luminance blocks)
 *
 *
 * RETURNS:     new Qscale
 *===========================================================================*/
extern int needQScaleChange (int oldQScale,  Block blk0, Block blk1, Block blk2, Block blk3);

/*===========================================================================*
 *
 * incNumBlocks()
 *
 *
 * RETURNS:   nothing
 *===========================================================================*/
extern void incNumBlocks (int num);


/*===========================================================================*
 *
 * incMacroBlockBits()
 *
 *  Increments the number of Macro Block bits and the total of Frame
 *  bits by the number passed.
 *
 * RETURNS:   nothing
 *===========================================================================*/
extern void incMacroBlockBits (int num);


/*===========================================================================*
 *
 * SetRateControl ()
 *
 *      Checks the string parsed from the parameter file.  Verifies
 *  number and sets global values.
 *
 * RETURNS:     nothing
 *===========================================================================*/
extern void SetRateControl (char *charPtr);


/*===========================================================================*
 *
 * setBufferSize ()
 *
 *      Checks the string parsed from the parameter file.  Verifies
 *  number and sets global values.
 *
 * RETURNS:     nothing
 *===========================================================================*/
extern void
setBufferSize(const char * const charPtr);


/*===========================================================================*
 *
 * getBufferSize ()
 *
 *      returns the buffer size read from the parameter file.  Size is
 *  in bits- not in units of 16k as written to the sequence header.
 *
 * RETURNS:     int (or -1 if invalid)
 *===========================================================================*/
extern int getBufferSize (void);


/*===========================================================================*
 *
 * setBitRate ()
 *
 *      Checks the string parsed from the parameter file.  Verifies
 *  number and sets global values.
 *
 * RETURNS:     nothing
 *
 * SIDE EFFECTS:   global variables
 *===========================================================================*/
extern void
setBitRate(const char * const charPtr);


/*===========================================================================*
 *
 * getBitRate ()
 *
 *      Returns the bit rate read from the parameter file.  This is the
 *  real rate in bits per second, not in 400 bit units as is written to
 *  the sequence header.
 *
 * RETURNS:     int (-1 if Variable mode operation)
 *===========================================================================*/
extern int getBitRate (void);


/*===========================================================================*
 *
 * getRateMode ()
 *
 *      Returns the rate mode- interpreted waa either Fixed or Variable
 *
 * RETURNS:     integer
 *===========================================================================*/
extern int getRateMode (void);


/*===========================================================================*
 *
 * incQuantOverride()
 *
 *  counter of override of quantization
 *
 * RETURNS:   nothing
 *===========================================================================*/
extern void incQuantOverride  (int num);

