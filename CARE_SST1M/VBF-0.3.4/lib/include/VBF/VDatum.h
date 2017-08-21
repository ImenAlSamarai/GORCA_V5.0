
/*
 * VDatum.h -- datum types
 * by Filip Pizlo, 2005
 */

#ifndef V_DATUM_H
#define V_DATUM_H

#include "FPIO.h"
#include "VConstants.h"
#include "VException.h"
#include "VEventType.h"
#include <sstream>

class VDatumException: public VException {};

class VDatumSizeInvalidException: public VDatumException {
 public:
    VDatumSizeInvalidException(const std::string &msg="")
    {
	setStrings("The length specified in the header is smaller "
		   "than the sum of the elements",msg);
    }
};

class VDatumInvalidException: public VDatumException {
 public:
    VDatumInvalidException(const std::string &msg="")
    {
	setStrings("Badly formatted Datum in stream",msg);
    }
};

class VDatumBadVersionException: public VDatumException {
 public:
    VDatumBadVersionException(const std::string &msg="")
    {
	setStrings("Bad version for the given operation",msg);
    }
};

class VDatumVersionParseException: public VDatumException {
 public:
    VDatumVersionParseException(const std::string &msg="")
    {
	setStrings("The given version string does not parse",msg);
    }
};

class VDatumAssertionException: public VDatumException {
 public:
    VDatumAssertionException(const std::string &msg="")
    {
	setStrings("Datum in stream violates assertions",msg);
    }
};

class VDatumParser;
class VDatumBuffer;

/*

VDatum Documention

The VDatum class combines the common elements of events (generated by telescopes)
and array triggers (generated by the L3).  It includes several interesting
features:

- Versioning.  There are two versions of the event spec, dubbed AUG 2004 and
  AUG 2005.  Using the VDatum class and its subclasses, it is possible to read
  and write both versions, and to 'upgrade' data from one version to the other.

- Copying.  It is easy to create a copy of any datum.  This is a 'deep copy', so
  the copy and the original are totally distinct and changes are not shared.

- Equality.  It is easy to check if two datums are identical.

- I/O to a variety of devices using the FPIO framework.  VBF interfaces to the
  FPIO framework to enable datums to be written to and read from files using
  a variety of formatting and compression schemes.

- Read and write mode.  A VDatum object can be used to create new data (as in
  data acquisition or simulation), edit existing data, or read data (as in
  data analysis).  The same API is used for both.

For reading data, you will use methods that begin with the word 'get', as in
'getEventNumber()', which returns a constant, or 'getGPSTime()', which returns
a pointer to the GPS time data.

For writing data, you will use methods that begin with either 'get' or 'set'.
If the data you are setting is a single element, as in the event number, you
will use a method like 'setEventNumber()'.  If the data consists of multiple
consecutive fields, like the GPS time, you will use 'getGPSTime()' to get a
pointer to those fields and then set them yourself.  For example, the code
'getGPSTime()[0] = x' will set the first element of the GPS time to x.

Note that versioning means that not all fields are available all at once.  Also,
some fields may have different sizes in different versions.  Methods that begin
with the word 'has', as in 'hasFlags()' tell you if a particular field is
available.  Methods that begin with the word 'is' answer questions about fields.
For example, if you want to know if the storage size of the flags field is
16 bits, you call the 'isFlags16Bit()' method.

One very notable difference between the 2004 and 2005 versions of the format is
that the 2005 version supports a 32-bit length field, allowing for very large
events.  The 2004 version only allows a 16-bit length field.

For documentation on specific fields, look for comments in the VDatum definition
below.  You should also look at the documentation for VEvent and VArrayTrigger
to find out more.

*/

class VDatum {
 public:
		
    enum Version {
	AUG_2004,
	AUG_2005
    };
    
    // answers the question: what version of the format are we using right
    // now?  Basically, when you create a new datum, its version is set
    // to currentVersion().
    static Version currentVersion() {
	return AUG_2005;
    }
    
    // answers the question: what is the latest, greatest, and not necessarily
    // final version of the format?  This method is totally unused and is
    // probably utterly useless, but I am too lazy to remove it.
    static Version latestVersion() {
	return AUG_2005;
    }
    
    // these methods allow you to stringify and destringify version numbers.
    static Version parseVersion(const std::string &str);
    static std::string versionToStr(Version version);
    
    static unsigned getGPSTimeNumElements() {
	return 5;
    }

 private:
		
    Version version;
		
    uword32 wilderness_size;
    uint8_t *wilderness;
	
    ubyte nodeNumber;
    ubyte triggerMask;
		
    // number of bytes: 4+2*5+1+1+4 = 20
    uword32 eventNumber;
    uword16 GPSTime[5];
    ubyte GPSYear;
    ubyte eventType;
    uword32 flags;
		
 protected:
		
    friend class VDatumParser;
    friend class VDatumBuffer;
		
    // these functions are just for reading/writing the body
		
    virtual uint8_t *readFromBufferImpl(uint8_t *cur,
					uint8_t *end,
					Version version);
		
    virtual uint8_t *writeToBufferImpl(uint8_t *cur);
		
    uint8_t *readFromBuffer(uint8_t *cur,
			    uint8_t *end,
			    Version version);
		
    uint8_t *writeToBuffer(uint8_t *cur);
		
    virtual bool isEqualToImpl(const VDatum *other) const;
		
    template< typename T >
    T *copyImpl() const {
	T *target=new T();
	target->nodeNumber=nodeNumber;
	target->triggerMask=triggerMask;
	target->eventNumber=eventNumber;
	target->wilderness_size=wilderness_size;
	if (wilderness!=NULL) {
	    target->wilderness=new uint8_t[wilderness_size];
	    memcpy(target->wilderness,wilderness,wilderness_size);
	} else {
	    target->wilderness=NULL;
	}
	memcpy(target->GPSTime,GPSTime,10);
	target->GPSYear=GPSYear;
	target->eventType=eventType;
	target->flags=flags;
	return target;
    }
		
 public:
    
    // create a new datum.  You aren't allowed to use this, actually.  You'll
    // get an error.  To construct a datum, you have to pick either VEvent or
    // VArrayTrigger.
    VDatum();
		
    virtual ~VDatum();
    
    // what is the total size in bytes of this datum?  That is: when I write this
    // datum to disk using VBankFileWriter, how many bytes will it occupy?
    // Note that this definition takes into account compression done by the VEvent,
    // but not compression like gzip or bzip2.
    virtual uword32 getSize() const;
    
    // ignore this method unless you really know what you're doing.
    void writeTo(FPOutputBase *output);
		
    // ignore this method unless you really know what you're doing.
    void readFrom(FPInputBase *input,
		  Version version);

    // create a deep copy of this datum.
    virtual VDatum *copy() const;
    
    // what version is this datum using?
    Version getVersion() const throw() {
	return version;
    }
    
    // _very_ dangerous method.  basically, it makes sense to call it after
    // creating an event or array trigger, but once you've started putting
    // data into this object, calling this method may seriously mess things
    // up.  (that is, you won't get a crash, but you may lose some data that
    // is represented differently between the old version and the new version.)
    void setVersion(Version version) throw() {
	this->version=version;
    }
    
    // is this datum equal to some other datum, bit for bit?
    bool isEqualTo(const VDatum *other) const;
    
    // Get or set the node number.  This is 255 for the array trigger.  For an
    // event, this is a 0-based telescope number.
    ubyte getNodeNumber() const throw() { return nodeNumber; }
    void setNodeNumber(ubyte nodeNumber) throw() { this->nodeNumber=nodeNumber; }
		
    // Get or set the DACQ trigger mask.  This value shouldn't necessarily be
    // trusted in the analysis, since simulations might not set it.
    ubyte getTriggerMask() const throw() { return triggerMask; }
    void setTriggerMask(ubyte triggerMask) throw() { this->triggerMask=triggerMask; }
    
    // the event number.
    uword32 getEventNumber() const throw() { return eventNumber; }
    void setEventNumber(uword32 eventNumber) throw() { this->eventNumber=eventNumber; }
    
    // the GPS time.  To set this, use getGPSTime() and then write into the pointer.
    const uword16 *getGPSTime() const throw() { return GPSTime; }
    uword16 *getGPSTime() throw() { return GPSTime; }
    
    // GPS year.  0 means 2000.
    ubyte getGPSYear() const throw() { return GPSYear; }
    void setGPSYear(ubyte GPSYear) throw() { this->GPSYear=GPSYear; }
    
    // Event types!  Amanda and I agreed to use the VEventType structure to
    // describe event types in general, since it will work with both types of
    // event type values.  (The AUG 2005 data will use a different set of types
    // than the AUG 2004 data.)  Check out the VEventType.h file for how that
    // works.
    //
    // Example: if you want to check if an event is a pedestal event, you should
    // do:
    //
    // if (ev->getEventType().trigger==VEventType::PED_TRIGGER) {
    //     ... pedestal event
    // } else {
    //     ... not pedestal event
    // }
    //
    // This code will work both for the 2004 data and the 2005 data.
    ubyte getRawEventTypeCode() const throw() { return eventType; }
    VEventType getEventType() const {
	VEventType evType;
	if (getVersion()==AUG_2004) {
	    evType.setOldStyleCode(eventType);
	} else {
	    evType.setNewStyleCode(eventType);
	}
	return evType;
    }
    ubyte getEventTypeCode() const {
	return getEventType().getBestNewStyleCode();
    }
    void setRawEventTypeCode(ubyte eventType) throw() { this->eventType=eventType; }
    void setEventTypeCode(ubyte eventType) throw() { this->eventType=eventType; }
    void setEventType(const VEventType &evType) {
	this->eventType=evType.getBestNewStyleCode();
    }
    
    // The flags field got really messed up for historical reasons.  In the 2004
    // data, the VEvent doesn't have a flags field and the VArrayTrigger has a
    // 32-bit flags field.  In the 2005 data, both have a 16-bit flags field.
    virtual bool hasFlags() =0;
    virtual bool isFlags16Bit() =0;
    virtual bool isFlags32Bit() =0;

    // get or set the flags.  Getting or setting the flags if hasFlags() returns
    // false will result in weirdness.  So don't do it.
    uword32 getFlags() const throw() { return flags; }
    virtual void setFlags(uword32 flags);
};

/*

VEvent Documention

The VEvent stores data generated by telescopes.  It is a variable-length structure
which you can resize dynamically.  Furthermore, it supports its own compression
scheme, which I call sample compression.  This scheme is entirely implemented in
VSampleCompressor.h and VSampleDecompressor.h.

There are several subtle differences between the 2004 and 2005 versions of this
structure:

- The 2004 version does not have a flags field.  The 2005 version does.

- The 2004 version stores the compressed bit as a highest-order bit in the numSamples
  field, while the 2005 version puts it in the lowest-order bit in the flags field.

*/

class VEvent: public VDatum {
 private:
    
    bool compressed;
    
    uword32 dotModule;
    
    uword16 numSamples;
    uword16 numChannels;
    uword16 maxNumChannels;
    uword16 numClockTrigBoards;
		
    uword32 *hitPattern;
    uword32 *triggerPattern;
		
    uword16 *pedsAndHiLo;
    uword16 *charges;
		
    ubyte *samples;
    
    mutable ubyte *cs;
    mutable unsigned csSize;
    mutable std::vector< uword16 > cpIndices;
    mutable bool csDirty;
		
    uword32 *clockTrigData;
    
    uint32_t selectedChan;

    mutable std::vector< uint32_t > hitVec;
    mutable bool hvDirty;
    
    void buildCS() const;
    void buildHitVec() const;
		
 protected:
		
    virtual uint8_t *readFromBufferImpl(uint8_t *cur,
					uint8_t *end,
					Version version);
		
    virtual uint8_t *writeToBufferImpl(uint8_t *cur);

    virtual bool isEqualToImpl(const VDatum *other) const;
		
 public:

    // creates an event with 0 samples, 0 channels, 0 channel bits, and
    // 0 clock trigger boards.
    VEvent();
    
    // this is the equivalent of using the VEvent() constructor and then
    // calling the appropriate resize methods (found below).
    VEvent(uword16 numSamples,
	   uword16 numChannels,
	   uword16 maxNumChannels,
	   uword16 numClockTrigBoards);
		
    virtual ~VEvent();
		
    VEvent *copyEvent() const;
    virtual VDatum *copy() const;
		
    virtual uword32 getSize() const;
    
    // The 2004 version doesn't have flags, while the 2005 has 16-bit flags.
    virtual bool hasFlags();
    virtual bool isFlags16Bit();
    virtual bool isFlags32Bit();
		
    uword32 getDOTModuleData() const {
	switch (getVersion()) {
	case AUG_2004:
	    return getFlags();
	case AUG_2005:
	    return dotModule;
	default:
	    V_FAIL("bad version");
	}
    }
    void setDOTModuleData(uword32 value) {
	switch (getVersion()) {
	case AUG_2004:
	    setFlags(value);
	    break;
	case AUG_2005:
	    dotModule=value;
	    break;
	default:
	    V_FAIL("bad version");
	}
    }
    
    // get the parameters that determine the size of this event.
    uword16 getNumSamples() const throw() { return numSamples; }
    uword16 getNumChannels() const throw() { return numChannels; }
    uword16 getMaxNumChannels() const throw() { return maxNumChannels; }
    uword16 getNumClockTrigBoards() const throw() { return numClockTrigBoards; }
    
    // get or set the bit that specifies whether compression is used.
    bool getCompressedBit() const throw() { return compressed; }
    void setCompressedBit(bool val) {
	compressed=val;
	if (getVersion()>=AUG_2005) {
	    if (compressed) {
		setFlags(getFlags()|1);
	    } else {
		setFlags(getFlags()&~1);
	    }
	}
    }
    
    virtual void setFlags(uword32 flags);
    
    // will compression be enabled for real?  even if the compressed bit is
    // set, compression will be disabled if the number of samples is 0, since
    // in this mode the compression algorithm will actually bloat the data
    // rather than compacting it.
    bool willUseCompression() const {
	return getCompressedBit()
	    && getNumSamples()!=0;
    }
    
    // these methods resize the event.  You may call them at any time; they are
    // safe to call even if you've already populated the event with data.
    void resizeChannelData(uword16 numSamples,
			   uword16 numChannels);
		
    void resizeChannelBits(uword16 maxNumChannels);
		
    void resizeClockTrigData(uword16 numClockTrigBoards);
    
    // what is the number of 32-bit words in the trigger and hit patterns?
    unsigned numBitPatternElements() const throw() {
	return (maxNumChannels+31)>>5;
    }
		
    uword32 *getHitPattern() throw() {
	hvDirty=true;
	return hitPattern;
    }
    const uword32 *getHitPattern() const throw() { return hitPattern; }
    bool getHitBit(uword32 channel) const throw() {
	return hitPattern[channel>>5]&(1<<(channel&31));
    }
    void setHitBit(unsigned channel,bool value) throw() {
	if (value) {
	    hitPattern[channel>>5]|=(1<<(channel&31));
	} else {
	    hitPattern[channel>>5]&=~(1<<(channel&31));
	}
	hvDirty=true;
    }
		
    uword32 *getTriggerPattern() throw() { return triggerPattern; }
    const uword32 *getTriggerPattern() const throw() { return triggerPattern; }
    bool getTriggerBit(unsigned channel) const throw() {
	return triggerPattern[channel>>5]&(1<<(channel&31));
    }
    void setTriggerBit(unsigned channel,bool value) throw() {
	if (value) {
	    triggerPattern[channel>>5]|=(1<<(channel&31));
	} else {
	    triggerPattern[channel>>5]&=~(1<<(channel&31));
	}
    }
    
    void verifyChannel(unsigned channel) const {
	V_ASSERT(channel<getNumChannels());
    }
        
    uword16 getPedestalAndHiLo(unsigned channel) const {
	verifyChannel(channel);
	return pedsAndHiLo[channel];
    }
    uword16 getPedestalAndHiLoWithoutVerify(unsigned channel) const {
	return pedsAndHiLo[channel];
    }
    void setPedestalAndHiLo(unsigned channel,uword16 value) {
	verifyChannel(channel);
	pedsAndHiLo[channel]=value;
	csDirty=true;
    }
        
    uword16 getPedestal(unsigned channel) const {
	return getPedestalAndHiLo(channel)&32767;
    }
    void setPedestal(unsigned channel,uword16 value) {
	setPedestalAndHiLo(channel,(getPedestalAndHiLo(channel)&32768)|(value&32767));
    }
        
    bool getHiLo(unsigned channel) const {
	return getPedestalAndHiLo(channel)&32768;
    }
    void setHiLo(unsigned channel,bool value) {
	setPedestalAndHiLo(channel,(getPedestalAndHiLo(channel)&32767)|(value?32768:0));
    }
        
    uword16 getCharge(unsigned channel) const {
	verifyChannel(channel);
	return charges[channel];
    }
    uword16 getChargeWithoutVerify(unsigned channel) const {
	return charges[channel];
    }
    void setCharge(unsigned channel,uword16 value) {
	verifyChannel(channel);
	charges[channel]=value;
	csDirty=true;
    }
        
    ubyte *getSamplePtr(unsigned channel,unsigned sample) {
	verifyChannel(channel);
	csDirty=true;
	return samples+channel*numSamples+sample;
    }
    const ubyte *getSamplePtr(unsigned channel,unsigned sample) const {
	verifyChannel(channel);
	return samples+channel*numSamples+sample;
    }
    ubyte getSample(unsigned channel,unsigned sample) const {
	verifyChannel(channel);
	return samples[channel*numSamples+sample];
    }
    void setSample(unsigned channel,unsigned sample,ubyte value) {
	verifyChannel(channel);
	csDirty=true;
	samples[channel*numSamples+sample]=value;
    }
		
    uword32 *getClockTrigData(unsigned clockTrigIndex) throw() {
	return clockTrigData+clockTrigIndex*7;
    }
    const uword32 *getClockTrigData(unsigned clockTrigIndex) const throw() {
	return clockTrigData+clockTrigIndex*7;
    }
    
    // methods from VRawEventParser that I've ported over

    uint32_t getHitID(uint32_t i) const;
    std::pair< bool, uint32_t > getChannelHitIndex(uint32_t chan) const;
    std::vector< bool > getFullHitVec() const;
    std::vector< bool > getFullTrigVec() const;
    uint16_t getMaxChannels() const { return getMaxNumChannels(); }
    uint16_t getNumChannelsHit() const { return getNumChannels(); }
    std::vector< uint8_t > getSamplesVec() const;
    void selectHitChan(uint32_t chan);

 protected:

    // the next two methods are only for the case where the event is
    // compressed.
    uword32 getEventPreSampleVarBodySize() const {
	return 2*4*((maxNumChannels+31)>>5);
    }
    uword32 getEventPostSampleVarBodySize() const {
	return 7*4*numClockTrigBoards+cpIndices.size()*4;
    }
    
    uword32 getEventVarBodySize() const {
	if (willUseCompression()) {
	    buildCS();
	    return getEventPreSampleVarBodySize()
		+ getEventPostSampleVarBodySize()
		+ ((csSize+3)&~3);
	} else {
	    return 2*4*((maxNumChannels+31)>>5)
		+ numChannels*(4+numSamples)
		+ 7*4*numClockTrigBoards;
	}
    }
};

/*

VArrayTrigger Documention

The VArrayTrigger contains fields from the L3.  (FIXME: add more documentation here!)

*/

class VArrayTrigger: public VDatum {
 private:
		
    uword16 ATflags;
    ubyte configMask;
    ubyte numSubarrayTelescopes;
    ubyte numTriggerTelescopes;
    uword32 runNumber;
		
    // some new fields
    uword32 tenMHzClock[4];
    uword32 optCalCount[3];
    uword32 pedCount[3];
    
    struct SubarrayTel {
	// fields that haven't changed
	uword32 telId;
	uword32 tdcTime;
	float azimuth;
	float altitude;

	// new fields
	uword32 evType;
	uword32 showerDelay;
	uword32 compDelay;
	uword32 l2Counts[3];
	uword32 calCounts[3];
	
	// deprecated fields
	uword32 delay;
	uword32 l2ScalarRate;
	uword32 l2Pattern[3];
	uword32 tenMhzClock;
	uword32 vetoedClock;
    };
		
    SubarrayTel *subarrayTels;
		
    struct TrigTel {
	uword32 telId;
    };
		
    TrigTel *trigTels;
		
 protected:
		
    virtual uint8_t *readFromBufferImpl(uint8_t *cur,
					uint8_t *end,
					Version version);
		
    virtual uint8_t *writeToBufferImpl(uint8_t *cur);

    virtual bool isEqualToImpl(const VDatum *other) const;
		
 public:
		
    VArrayTrigger();
		
    VArrayTrigger(ubyte numSubarrayTelescopes,
		  ubyte numTriggerTelescopes);
		
    virtual ~VArrayTrigger();
		
    VArrayTrigger *copyAT() const;
    virtual VDatum *copy() const;
		
    virtual uword32 getSize() const;
		
    virtual bool hasFlags();
    virtual bool isFlags16Bit();
    virtual bool isFlags32Bit();
    
    ubyte getNumSubarrayTelescopes() const throw() { return numSubarrayTelescopes; }
    ubyte getNumTriggerTelescopes() const throw() { return numTriggerTelescopes; }
		
    void resizeSubarrayTelescopes(ubyte numSubarrayTelescopes);
    void resizeTriggerTelescopes(ubyte numTriggerTelescopes);
		
    uword16 getATFlags() const throw() { return ATflags; }
    void setATFlags(uword16 value) throw() { ATflags=value; }
		
    bool isATFlags8Bit() const throw() { return getVersion()>AUG_2004; }
		
    ubyte getConfigMask() const throw() { return configMask; }
    void setConfigMask(ubyte value) throw() { configMask=value; }
		
    uword32 getRunNumber() const throw() { return runNumber; }
    void setRunNumber(uword32 value) throw() { runNumber=value; }
		
    bool hasTenMHzClockArray() const { return getVersion()>=AUG_2005; }
    bool hasOptCalCountArray() const { return getVersion()>=AUG_2005; }
    bool hasPedCountArray() const { return getVersion()>=AUG_2005; }
    
    uword32 *getTenMHzClockArray() { return tenMHzClock; }
    uword32 *getOptCalCountArray() { return optCalCount; }
    uword32 *getPedCountArray() { return pedCount; }
    const uword32 *getTenMHzClockArray() const { return tenMHzClock; }
    const uword32 *getOptCalCountArray() const { return optCalCount; }
    const uword32 *getPedCountArray() const { return pedCount; }
		
    uword32 getSubarrayTelescopeId(unsigned index) const throw() {
	return subarrayTels[index].telId;
    }
    void setSubarrayTelescopeId(unsigned index,uword32 value) throw() {
	subarrayTels[index].telId=value;
    }
    uword32 getTDCTime(unsigned index) const throw() {
	return subarrayTels[index].tdcTime;
    }
    void setTDCTime(unsigned index,uword32 value) throw() {
	subarrayTels[index].tdcTime=value;
    }
    uword32 getSpecificRawEventTypeCode(unsigned index) const throw() {
	return subarrayTels[index].evType;
    }
    uword32 getSpecificEventTypeCode(unsigned index) const throw() {
	return subarrayTels[index].evType;
    }
    VEventType getSpecificRawEventType(unsigned index) const {
	VEventType evType;
	evType.setNewStyleCode((ubyte)subarrayTels[index].evType);
	return evType;
    }
    void setSpecificEventTypeCode(unsigned index,uword32 value) throw() {
	subarrayTels[index].evType=value;
    }
    void setSpecificRawEventTypeCode(unsigned index,uword32 value) throw() {
	subarrayTels[index].evType=value;
    }
    void setSpecificEventType(unsigned index,const VEventType &value) {
	subarrayTels[index].evType=value.getBestNewStyleCode();
    }
    bool hasSpecificEventType() const throw() {
	return getVersion()>AUG_2004;
    }
    float getAzimuth(unsigned index) const throw() {
	return subarrayTels[index].azimuth;
    }
    void setAzimuth(unsigned index,float value) throw() {
	subarrayTels[index].azimuth=value;
    }
    float getAltitude(unsigned index) const throw() {
	return subarrayTels[index].altitude;
    }
    void setAltitude(unsigned index,float value) throw() {
	subarrayTels[index].altitude=value;
    }
    uword32 getShowerDelay(unsigned index) const throw() {
	return subarrayTels[index].showerDelay;
    }
    void setShowerDelay(unsigned index,uword32 value) throw() {
	subarrayTels[index].showerDelay=value;
    }
    bool hasShowerDelay() const throw() {
	return getVersion()>AUG_2004;
    }
    uword32 getCompDelay(unsigned index) const throw() {
	return subarrayTels[index].compDelay;
    }
    void setCompDelay(unsigned index,uword32 value) throw() {
	subarrayTels[index].compDelay=value;
    }
    bool hasCompDelay() const throw() {
	return getVersion()>AUG_2004;
    }
		
    bool hasL2CountsArray() const { return getVersion()>=AUG_2005; }
    bool hasCalCountsArray() const { return getVersion()>=AUG_2005; }
    uword32 *getL2CountsArray(unsigned i) { return subarrayTels[i].l2Counts; }
    uword32 *getCalCountsArray(unsigned i) { return subarrayTels[i].calCounts; }
    const uword32 *getL2CountsArray(unsigned i) const { return subarrayTels[i].l2Counts; }
    const uword32 *getCalCountsArray(unsigned i) const { return subarrayTels[i].calCounts; }
    
    uword32 getDelay(unsigned index) const throw() {
	return subarrayTels[index].delay;
    }
    void setDelay(unsigned index,uword32 val) throw() {
	subarrayTels[index].delay=val;
    }
    bool hasDelay() const throw() {
	return getVersion()==AUG_2004;
    }
    uword32 getL2ScalarRate(unsigned index) const throw() {
	return subarrayTels[index].l2ScalarRate;
    }
    void setL2ScalarRate(unsigned index,uword32 val) throw() {
	subarrayTels[index].l2ScalarRate=val;
    }
    bool hasL2ScalarRate() const throw() {
	return getVersion()==AUG_2004;
    }
    uword32 *getL2Pattern(unsigned i) throw() {
	return subarrayTels[i].l2Pattern;
    }
    const uword32 *getL2Pattern(unsigned i) const throw() {
	return subarrayTels[i].l2Pattern;
    }
    bool hasL2Pattern() const throw() {
	return getVersion()==AUG_2004;
    }
    uword32 getTenMhzClock(unsigned index) const throw() {
	return subarrayTels[index].tenMhzClock;
    }
    void setTenMhzClock(unsigned index,uword32 val) throw() {
	subarrayTels[index].tenMhzClock=val;
    }
    bool hasTenMhzClock() const throw() {
	return getVersion()==AUG_2004;
    }
    uword32 getVetoedClock(unsigned index) const throw() {
	return subarrayTels[index].vetoedClock;
    }
    void setVetoedClock(unsigned index,uword32 val) throw() {
	subarrayTels[index].vetoedClock=val;
    }
    bool hasVetoedClock() const throw() {
	return getVersion()==AUG_2004;
    }
		
    uword32 getTriggerTelescopeId(unsigned index) const throw() {
	return trigTels[index].telId;
    }
    void setTriggerTelescopeId(unsigned index,uword32 value) throw() {
	trigTels[index].telId=value;
    }

 protected:

    uword32 getATVarBodySize() const {
	switch (getVersion()) {
	case AUG_2004:
	    return 44*numSubarrayTelescopes;
	case AUG_2005:
	    return (4+4+4+4+4+4+4+4*3+4*3)*numSubarrayTelescopes
		+ 4*numTriggerTelescopes;
	default:
	    V_FAIL("bad version");
	}
    }
};

// this class is really for internal use.  don't use it unless you know
// what you're doing.
class VDatumParser {
 private:
    VDatumParser();
    VDatumParser(const VDatumParser&);
    void operator=(const VDatumParser&);
		
 public:
		
    static VDatum *buildFrom(FPInputBase *input,
			     VDatum::Version version);
};

#endif


// **************************************************************************
// Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 
// 2011, 2012 Purdue University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//  this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are 
// those of the authors and should not be interpreted as representing official
// policies, either expressed or implied, of Purdue University or the VERITAS 
// Collaboration.
// **************************************************************************