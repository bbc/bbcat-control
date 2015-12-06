#ifndef __AUDIO_OBJECT_PARAMETERS__
#define __AUDIO_OBJECT_PARAMETERS__

#include <bbcat-base/3DPosition.h>
#include <bbcat-base/NamedParameter.h>
#include <bbcat-base/ParameterSet.h>
#include <bbcat-base/RefCount.h>

BBC_AUDIOTOOLBOX_START

/*--------------------------------------------------------------------------------*/
/** A class containing the parameters for rendering audio objects
 *
 * Each input channel to the renderer requires one of these objects which may change
 * over time as parameters for that channel change
 */
/*--------------------------------------------------------------------------------*/
class AudioObject;
class AudioObjectParameters
{
public:
  AudioObjectParameters();
  AudioObjectParameters(const AudioObjectParameters& obj);
#if ENABLE_JSON
  AudioObjectParameters(const json_spirit::mObject& obj);
#endif
  virtual ~AudioObjectParameters();

  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& operator = (const AudioObjectParameters& obj);

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& operator = (const json_spirit::mObject& obj) {return FromJSON(obj);}
  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& FromJSON(const json_spirit::mObject& obj, bool reset = true);
#endif

  /*--------------------------------------------------------------------------------*/
  /** Comparison operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool operator == (const AudioObjectParameters& obj) const;
  virtual bool operator != (const AudioObjectParameters& obj) const {return !operator == (obj);}

  /*--------------------------------------------------------------------------------*/
  /** Merge another AudioObjectParameters into this one
   *
   * @note any values set in obj will over-write the ones in this object
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& Merge(const AudioObjectParameters& obj);
  
  /*--------------------------------------------------------------------------------*/
  /** Transform this object's position and return new copy
   */
  /*--------------------------------------------------------------------------------*/
  friend AudioObjectParameters operator * (const AudioObjectParameters& obj, const PositionTransform& transform);

  /*--------------------------------------------------------------------------------*/
  /** Transform this object's position
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& operator *= (const PositionTransform& transform);

  /*--------------------------------------------------------------------------------*/
  /** Get/Set channel
   */
  /*--------------------------------------------------------------------------------*/
  uint_t GetChannel()            const {return values.channel;}
  bool   GetChannel(uint_t& val) const {return GetParameter<>(Parameter_channel, values.channel, val);}
  void   SetChannel(uint_t  val)       {SetParameter<>(Parameter_channel, values.channel, val);}
  void   ResetChannel()                {ResetParameter<>(Parameter_channel, values.channel);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set block duration (ns by default)
   */
  /*--------------------------------------------------------------------------------*/
  uint64_t GetDuration()              const {return values.duration;}
  double   GetDurationS()             const {return ConvertNSToS(values.duration);}
  bool     GetDuration(uint64_t& val) const {return GetParameter<>(Parameter_duration, values.duration, val);}
  bool     GetDurationS(double&  val) const {return GetParameter<>(Parameter_duration, values.duration, val, &ConvertNSToS);}
  void     SetDuration(uint64_t  val)       {SetParameter<>(Parameter_duration, values.duration, val);}
  void     SetDurationS(double   val)       {SetParameter<>(Parameter_duration, values.duration, val, &ConvertSToNS);}
  void     ResetDuration()                  {ResetParameter<>(Parameter_duration, values.duration);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set physical position of this object
   *
   * @note position information is required for every channel
   */
  /*--------------------------------------------------------------------------------*/
  const Position& GetPosition()                    const {return position;}
  bool            GetPosition(Position& val)       const {return GetParameter<>(Parameter_position, position, val);}
  void            SetPosition(const Position& val)       {SetParameter<>(Parameter_position, position, val);}
  void            ResetPosition()                        {ResetParameter<>(Parameter_position, position);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set gain
   */
  /*--------------------------------------------------------------------------------*/
  double GetGain()            const {return values.gain;}
  bool   GetGain(double& val) const {return GetParameter<>(Parameter_gain, values.gain, val);}
  void   SetGain(double  val)       {SetParameter<>(Parameter_gain, values.gain, val);}
  void   ResetGain()                {ResetParameter<>(Parameter_gain, values.gain, 1.0);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set width
   */
  /*--------------------------------------------------------------------------------*/
  float  GetWidth()           const {return values.width;}
  bool   GetWidth(float& val) const {return GetParameter<>(Parameter_width, values.width, val);}
  void   SetWidth(float  val)       {SetParameter<>(Parameter_width, values.width, val, &Limit0f);}
  void   ResetWidth()               {ResetParameter<>(Parameter_width, values.width);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set depth
   */
  /*--------------------------------------------------------------------------------*/
  float  GetDepth()           const {return values.depth;}
  bool   GetDepth(float& val) const {return GetParameter<>(Parameter_depth, values.depth, val);}
  void   SetDepth(float  val)       {SetParameter<>(Parameter_depth, values.depth, val, &Limit0f);}
  void   ResetDepth()               {ResetParameter<>(Parameter_depth, values.depth);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set height
   */
  /*--------------------------------------------------------------------------------*/
  float  GetHeight()           const {return values.height;}
  bool   GetHeight(float& val) const {return GetParameter<>(Parameter_height, values.height, val);}
  void   SetHeight(float  val)       {SetParameter<>(Parameter_height, values.height, val, &Limit0f);}
  void   ResetHeight()               {ResetParameter<>(Parameter_height, values.height);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set divergence balance
   */
  /*--------------------------------------------------------------------------------*/
  float  GetDivergenceBalance()           const {return values.divergencebalance;}
  bool   GetDivergenceBalance(float& val) const {return GetParameter<>(Parameter_divergencebalance, values.divergencebalance, val);}
  void   SetDivergenceBalance(float  val)       {SetParameter<>(Parameter_divergencebalance, values.divergencebalance, val, &Limit0to1f);}
  void   ResetDivergenceBalance()               {ResetParameter<>(Parameter_divergencebalance, values.divergencebalance);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set divergence azimuth
   */
  /*--------------------------------------------------------------------------------*/
  float  GetDivergenceAzimuth()           const {return values.divergenceazimuth;}
  bool   GetDivergenceAzimuth(float& val) const {return GetParameter<>(Parameter_divergenceazimuth, values.divergenceazimuth, val);}
  void   SetDivergenceAzimuth(float  val)       {SetParameter<>(Parameter_divergenceazimuth, values.divergenceazimuth, val, &Limit0f);}
  void   ResetDivergenceAzimuth()               {ResetParameter<>(Parameter_divergenceazimuth, values.divergenceazimuth);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set diffuseness
   */
  /*--------------------------------------------------------------------------------*/
  float  GetDiffuseness()           const {return values.diffuseness;}
  bool   GetDiffuseness(float& val) const {return GetParameter<>(Parameter_diffuseness, values.diffuseness, val);}
  void   SetDiffuseness(float  val)       {SetParameter<>(Parameter_diffuseness, values.diffuseness, val, Limit0to1f);}
  void   ResetDiffuseness()               {ResetParameter<>(Parameter_diffuseness, values.diffuseness);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set delay
   */
  /*--------------------------------------------------------------------------------*/
  float  GetDelay()           const {return values.delay;}
  bool   GetDelay(float& val) const {return GetParameter<>(Parameter_delay, values.delay, val);}
  void   SetDelay(float  val)       {SetParameter<>(Parameter_delay, values.delay, val, &Limit0f);}
  void   ResetDelay()               {ResetParameter<>(Parameter_delay, values.delay);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set importance
   */
  /*--------------------------------------------------------------------------------*/
  int    GetImportance()         const {return values.importance;}
  bool   GetImportance(int& val) const {return GetParameter<>(Parameter_importance, values.importance, val);}
  void   SetImportance(int  val)       {SetParameter<>(Parameter_importance, values.importance, val, &LimitImportance);}
  void   ResetImportance()             {ResetParameter<>(Parameter_importance, values.importance);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set dialogue
   */
  /*--------------------------------------------------------------------------------*/
  int    GetDialogue()         const {return values.dialogue;}
  bool   GetDialogue(int& val) const {return GetParameter<>(Parameter_dialogue, values.dialogue, val);}
  void   SetDialogue(int  val)       {SetParameter<>(Parameter_dialogue, values.dialogue, val, &LimitDialogue);}
  void   ResetDialogue()             {ResetParameter<>(Parameter_dialogue, values.dialogue);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set channellock
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetChannelLock()          const {return (values.channellock != 0);}
  bool   GetChannelLock(bool& val) const {return GetBoolParameter<>(Parameter_channellock, values.channellock, val);}
  void   SetChannelLock(bool  val)       {SetParameter<>(Parameter_channellock, values.channellock, val);}
  void   ResetChannelLock()              {ResetParameter<>(Parameter_channellock, values.channellock);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set channellock maxdistance
   */
  /*--------------------------------------------------------------------------------*/
  float  GetChannelLockMaxDistance()           const {return values.channellockmaxdistance;}
  bool   GetChannelLockMaxDistance(float& val) const {return GetParameter<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, val);}
  void   SetChannelLockMaxDistance(float  val)       {SetParameter<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, val, LimitMaxDistance);}
  void   ResetChannelLockMaxDistance()               {ResetParameter<>(Parameter_channellockmaxdistance, values.channellockmaxdistance);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interact
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetInteract()          const {return (values.interact != 0);}
  bool   GetInteract(bool& val) const {return GetBoolParameter<>(Parameter_interact, values.interact, val);}
  void   SetInteract(bool  val)       {SetParameter<>(Parameter_interact, values.interact, val);}
  void   ResetInteract()              {ResetParameter<>(Parameter_interact, values.interact);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interpolate
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetInterpolate()          const {return (values.interpolate != 0);}
  bool   GetInterpolate(bool& val) const {return GetBoolParameter<>(Parameter_interpolate, values.interpolate, val);}
  void   SetInterpolate(bool  val)       {SetParameter<>(Parameter_interpolate, values.interpolate, val);}
  void   ResetInterpolate()              {ResetParameter<>(Parameter_interpolate, values.interpolate);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interpolation time (ns by default)
   */
  /*--------------------------------------------------------------------------------*/
  uint64_t GetInterpolationTime()              const {return values.interpolationtime;}
  double   GetInterpolationTimeS()             const {return ConvertNSToS(values.interpolationtime);}
  bool     GetInterpolationTime(uint64_t& val) const {return GetParameter<>(Parameter_interpolationtime, values.interpolationtime, val);}
  bool     GetInterpolationTimeS(double&  val) const {return GetParameter<>(Parameter_interpolationtime, values.interpolationtime, val, &ConvertNSToS);}
  void     SetInterpolationTime(uint64_t  val)       {SetParameter<>(Parameter_interpolationtime, values.interpolationtime, val);}
  void     SetInterpolationTimeS(double   val)       {SetParameter<>(Parameter_interpolationtime, values.interpolationtime, val, &ConvertSToNS);}
  void     ResetInterpolationTime()                  {ResetParameter<>(Parameter_interpolationtime, values.interpolationtime);}

  /*--------------------------------------------------------------------------------*/
  /** Return actual interpolation time in ns according to interpolate, interpolationtime and duration (according to BS 2076)
   */
  /*--------------------------------------------------------------------------------*/
  uint64_t GetActualInterpolationTime() const {return GetInterpolate() ? GetInterpolationTime() : GetDuration();}
  
  /*--------------------------------------------------------------------------------*/
  /** Get/Set onscreen
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetOnScreen()          const {return (values.onscreen != 0);}
  bool   GetOnScreen(bool& val) const {return GetBoolParameter<>(Parameter_onscreen, values.onscreen, val);}
  void   SetOnScreen(bool val)        {SetParameter<>(Parameter_onscreen, values.onscreen, val);}
  void   ResetOnScreen()              {ResetParameter<>(Parameter_onscreen, values.onscreen);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set supplementary data
   */
  /*--------------------------------------------------------------------------------*/
  const ParameterSet& GetOtherValues()                        const {return othervalues;}
  bool                GetOtherValues(ParameterSet& val)       const {return GetParameter<>(Parameter_othervalues, othervalues, val);}
  void                SetOtherValues(const ParameterSet& val)
  {
    if (val.IsEmpty()) ResetParameter<>(Parameter_othervalues, othervalues);
    else               SetParameter<>(Parameter_othervalues, othervalues, val);
  }
  void                ResetOtherValues()                            {ResetParameter<>(Parameter_othervalues, othervalues);}

  /*--------------------------------------------------------------------------------*/
  /** Sub class describing an excluded zone supporting the zoneExclusion ADM parameter
   */
  /*--------------------------------------------------------------------------------*/
  class ExcludedZone
  {
  public:
	ExcludedZone() : next(NULL),
					 minx(0.0),
					 miny(0.0),
					 minz(0.0),
					 maxx(0.0),
					 maxy(0.0),
					 maxz(0.0) {}
	ExcludedZone(ExcludedZone& obj)
	{
	  name = obj.name;
	  minx = obj.minx;
	  maxx = obj.maxx;
	  miny = obj.miny;
	  maxy = obj.maxy;
	  minz = obj.minz;
	  maxz = obj.maxz;
	  // also copy child object (which will in turn copy its child and so on)
	  if (obj.next) next = new ExcludedZone(*obj.next);
	  else			next = NULL;	// end of chain
	}
	~ExcludedZone() {if (next) delete next;}	// automatic deletion of child object

	/*--------------------------------------------------------------------------------*/
	/** Comparison operator
	 */
	/*--------------------------------------------------------------------------------*/
	bool operator == (const ExcludedZone& zone) const
	{
	  return ((name == zone.name) &&
			  (minx == zone.minx) &&
			  (miny == zone.miny) &&
			  (minz == zone.minz) &&
			  (maxx == zone.maxx) &&
			  (maxy == zone.maxy) &&
			  (maxz == zone.maxz));
	}

	/*--------------------------------------------------------------------------------*/
	/** Compare two chains of excluded zones
	 */
	/*--------------------------------------------------------------------------------*/
	friend bool Compare(const ExcludedZone *zone1, const ExcludedZone *zone2) {
	  return ((!zone1 && !zone2) ||															// end of BOTH chains; or
			  (zone1 && zone2 && (*zone1 == *zone2) && Compare(zone1->next, zone2->next)));	// both are valid and match and next on each match
	}
	
	/*--------------------------------------------------------------------------------*/
	/** Find equivalent of supplied zone in list
	 */
	/*--------------------------------------------------------------------------------*/
	const ExcludedZone *Find(const ExcludedZone& zone) const {return (zone == *this) ? this : (next ? next->Find(zone) : NULL);}
	  
	/*--------------------------------------------------------------------------------*/
	/** Set name
	 */
	/*--------------------------------------------------------------------------------*/
	void SetName(const std::string& _name) {name = _name;}
	
	/*--------------------------------------------------------------------------------*/
	/** Set minimum corner
	 */
	/*--------------------------------------------------------------------------------*/
	void SetMinCorner(float x, float y, float z) {minx = x; miny = y; minz = z;}

	/*--------------------------------------------------------------------------------*/
	/** Set maximum corner
	 */
	/*--------------------------------------------------------------------------------*/
	void SetMaxCorner(float x, float y, float z) {maxx = x; maxy = y; maxz = z;}
	
	/*--------------------------------------------------------------------------------*/
	/** Add another excluded zone to the END of the list
	 */
	/*--------------------------------------------------------------------------------*/
	void Add(ExcludedZone *zone)
	{
	  // if this object is not the end of the chain, pass the zone to the next in the list
	  if (next) next->Add(zone);
	  // otherwise set the next item
	  else      next = zone;
	}

	/*--------------------------------------------------------------------------------*/
	/** Get name
	 */
	/*--------------------------------------------------------------------------------*/
	const std::string& GetName() const {return name;}

	/*--------------------------------------------------------------------------------*/
	/** Return position of specified corner
	 *
	 * @param n corner number (bit 0: min/max x, bit 1: min/max y, bit 2: min/max z)
	 *
	 */
	/*--------------------------------------------------------------------------------*/
	Position GetCorner(uint_t n) const {
	  return Position((n & 1) ? maxx : minx,
					  (n & 2) ? maxy : miny,
					  (n & 4) ? maxz : minz);
	}

	/*--------------------------------------------------------------------------------*/
	/** Return position of minimum corner
	 */
	/*--------------------------------------------------------------------------------*/
	Position GetMinCorner() const {return GetCorner(0);}

	/*--------------------------------------------------------------------------------*/
	/** Return position of maximum corner
	 */
	/*--------------------------------------------------------------------------------*/
	Position GetMaxCorner() const {return GetCorner(7);}

	/*--------------------------------------------------------------------------------*/
	/** Return whether position is within this or ANY child zone
	 */
	/*--------------------------------------------------------------------------------*/
	bool Within(const Position& _pos) const {
	  Position pos = _pos.Cart();
	  return ((RANGE(pos.pos.x, minx, maxx) &&
			   RANGE(pos.pos.y, miny, maxy) &&
			   RANGE(pos.pos.z, minx, maxz)) || (next && next->Within(pos)));
	}

	/*--------------------------------------------------------------------------------*/
	/** Return next excluded zone in the list
	 */
	/*--------------------------------------------------------------------------------*/
	const ExcludedZone *GetNext() const {return next;}
	
  protected:
	ExcludedZone *next;
	std::string name;
	float minx, miny, minz;
	float maxx, maxy, maxz;
  };

  /*--------------------------------------------------------------------------------*/
  /** Add a single excluded zone to list
   */
  /*--------------------------------------------------------------------------------*/
  virtual void AddExcludedZone(const std::string& name, float minx, float miny, float minz, float maxx, float maxy, float maxz);

  /*--------------------------------------------------------------------------------*/
  /** Delete all excluded zones
   */
  /*--------------------------------------------------------------------------------*/
  virtual void ResetExcludedZones();

  /*--------------------------------------------------------------------------------*/
  /** Return whether supplied position is any of the excluded zones
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool PositionWithinExcludedZones(const Position& pos) const {return excludedZones ? excludedZones->Within(pos) : false;}

  /*--------------------------------------------------------------------------------*/
  /** Return first excluded zone
   */
  /*--------------------------------------------------------------------------------*/
  const ExcludedZone *GetFirstExcludedZone() const {return excludedZones;}
  
  /*--------------------------------------------------------------------------------*/
  /** Convert all parameters into text and store them in a ParameterSet object 
   *
   * @param set ParameterSet object to receive parameters
   */
  /*--------------------------------------------------------------------------------*/
  virtual void GetAll(ParameterSet& set, bool force = false) const;

  /*--------------------------------------------------------------------------------*/
  /** Get a list of parameters for this object
   */
  /*--------------------------------------------------------------------------------*/
  static void GetParameterDescriptions(std::vector<const PARAMETERDESC *>& list);

  /*--------------------------------------------------------------------------------*/
  /** Get a list of overrideable parameters for this object
   */
  /*--------------------------------------------------------------------------------*/
  static void GetOverrideableParameterDescriptions(std::vector<const PARAMETERDESC *>& list);

  /*--------------------------------------------------------------------------------*/
  /** Position needs to be handled different so this function returns the textual name of the position control
   */
  /*--------------------------------------------------------------------------------*/
  static const PARAMETERDESC& GetPositionDesc() {return parameterdescs[Parameter_position];}
  
  /*--------------------------------------------------------------------------------*/
  /** Set parameter from string
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool SetValue(const std::string& name, const std::string& value);

  /*--------------------------------------------------------------------------------*/
  /** Get parameter as string
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool GetValue(const std::string& name, std::string& value) const;

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter by name
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool ResetValue(const std::string& name);
  
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a string
   */
  /*--------------------------------------------------------------------------------*/
  std::string ToString(bool pretty = false) const;

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON object
   */
  /*--------------------------------------------------------------------------------*/
  virtual void ToJSON(json_spirit::mObject& obj, bool force = false) const;

  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON object
   */
  /*--------------------------------------------------------------------------------*/
  virtual json_spirit::mObject ToJSON(bool force = false) const {json_spirit::mObject obj; ToJSON(obj, force); return obj;}

  /*--------------------------------------------------------------------------------*/
  /** Operator overload
   */
  /*--------------------------------------------------------------------------------*/
  operator json_spirit::mObject() const {return ToJSON();}
 
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON string
   */
  /*--------------------------------------------------------------------------------*/
  std::string ToJSONString() const {return json_spirit::write(ToJSON(), json_spirit::pretty_print);}
#endif

  /*--------------------------------------------------------------------------------*/
  /** Parameter modifier class
   */
  /*--------------------------------------------------------------------------------*/
  class Modifier : public RefCountedObject
  {
  public:
    Modifier() : RefCountedObject() {}
    Modifier(const Modifier& obj);
    virtual ~Modifier() {}

    NAMEDPARAMETER(Quaternion,rotation);
    NAMEDPARAMETER(Position,position);
    NAMEDPARAMETERDEF(double,gain,1.0);
    NAMEDPARAMETERDEF(double,scale,1.0);

    typedef std::vector<RefCount<Modifier> > LIST;

    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& operator = (const Modifier& obj);

    /*--------------------------------------------------------------------------------*/
    /** Comparison operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual bool operator == (const Modifier& obj) const;

    /*--------------------------------------------------------------------------------*/
    /** Specific modifications
     */
    /*--------------------------------------------------------------------------------*/
    virtual void Modify(AudioObjectParameters& parameters, const AudioObject *object = NULL) const;

#if ENABLE_JSON
    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& operator = (const json_spirit::mObject& obj) {return FromJSON(obj);}
    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& FromJSON(const json_spirit::mObject& obj);

    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON object
     */
    /*--------------------------------------------------------------------------------*/
    virtual void ToJSON(json_spirit::mObject& obj) const;

    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON object
     */
    /*--------------------------------------------------------------------------------*/
    virtual json_spirit::mObject ToJSON() const {json_spirit::mObject obj; ToJSON(obj); return obj;}

    /*--------------------------------------------------------------------------------*/
    /** Operator overload
     */
    /*--------------------------------------------------------------------------------*/
    operator json_spirit::mObject() const {return ToJSON();}
 
    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON string
     */
    /*--------------------------------------------------------------------------------*/
    std::string ToJSONString() const {return json_spirit::write(ToJSON(), json_spirit::pretty_print);}
#endif
  };

  /*--------------------------------------------------------------------------------*/
  /** Modify this object's parameters using a single modifier
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& Modify(const Modifier& modifier, const AudioObject *object);

  /*--------------------------------------------------------------------------------*/
  /** Modify this object's parameters using a list of modifiers
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& Modify(const Modifier::LIST& list, const AudioObject *object);

protected:
  void GetList(std::vector<INamedParameter *>& list);
  void InitialiseToDefaults();

  /*--------------------------------------------------------------------------------*/
  /** List of parameters for 'setbitmap'
   *
   * @note this list is COMPLETELY INDEPENDANT of the VALUES structure and the order is ONLY relevant to parameterdescs[]
   */
  /*--------------------------------------------------------------------------------*/
  typedef enum {
    Parameter_channel = 0,
    Parameter_duration,
    
    Parameter_position,

    Parameter_gain,

    Parameter_width,
    Parameter_height,
    Parameter_depth,

    Parameter_divergencebalance,
    Parameter_divergenceazimuth,

    Parameter_diffuseness,
    Parameter_delay,

    Parameter_importance,
    Parameter_dialogue,

    Parameter_channellock,
    Parameter_channellockmaxdistance,
    Parameter_interact,
    Parameter_interpolate,
    Parameter_interpolationtime,
    Parameter_onscreen,

    Parameter_othervalues,
    
    Parameter_count,
  } Parameter_t;

  /*--------------------------------------------------------------------------------*/
  /** Limit functions for various parameters
   */
  /*--------------------------------------------------------------------------------*/
  static uint8_t  LimitImportance(const int& val)     {return LIMIT(val, 0, 10);}
  static uint8_t  LimitDialogue(const int& val)       {return LIMIT(val, 0, 2);}
  static uint8_t  LimitBool(const int& val)           {return (val != 0);}
  static uint_t   Limit0u(const int& val)             {return MAX(val, 0);}              // at least 0
  static uint64_t Limit0u64(const sint64_t& val)      {return MAX(val, 0);}              // at least 0
  static double   Limit0(const double& val)           {return MAX(val, 0.0);}            // at least 0
  static float    Limit0f(const float& val)           {return MAX(val, 0.f);}            // at least 0
  static double   Limit0to1(const double& val)        {return LIMIT(val, 0.0, 1.0);}     // between 0 and 1
  static float    Limit0to1f(const float& val)        {return LIMIT(val, 0.f, 1.f);}     // between 0 and 1
  static float    LimitMaxDistance(const float& val)  {return LIMIT(val, 0.f, 2.f);}     // between 0 and 2
  static uint64_t ConvertSToNS(const double& val)     {return (uint64_t)MAX(val * 1.0e9, 0.0);}
  static double   ConvertNSToS(const uint64_t& val)   {return (double)val * 1.0e-9;}
  
  /*--------------------------------------------------------------------------------*/
  /** Set parameter from value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val new value
   * @param limit optional function ptr to a limiting/converting function that will convert the supplied value to a usable parameter value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetParameter(Parameter_t p, T1& param, const T2& val, T1 (*limit)(const T2& val) = NULL) {param = limit ? (*limit)(val) : T1(val); MarkParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Get parameter to value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value to be return
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter source
   * @param val destination value
   *
   * @return true if value has been set, false if it is at its default
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  bool GetParameter(Parameter_t p, const T1& param, T2& val, T2 (*convert)(const T1& val) = NULL) const {val = convert ? (*convert)(param) : T2(param); return IsParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Get parameter to bool value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value to be return
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter source
   * @param val destination value
   *
   * @return true if value has been set, false if it is at its default
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool GetBoolParameter(Parameter_t p, const T1& param, bool& val) const {val = (param != 0); return IsParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of reset value
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val reset value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void ResetParameter(Parameter_t p, T1& param, const T2& val) {param = val; MarkParameterReset(p);}

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter to 'zero'
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  void ResetParameter(Parameter_t p, T1& param) {param = T1(); MarkParameterReset(p);}

  /*--------------------------------------------------------------------------------*/
  /** Set parameter in ParameterSet from specified parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param parameters ParameterSet object to be populated
   * @param force true to force parameters to be set, even if they have not been set
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool GetParameterFromParameters(Parameter_t p, const T1& param, ParameterSet& parameters, bool force = false) const {
    if (force || IsParameterSet(p)) parameters.Set(parameterdescs[p].name, param);
    return (force || IsParameterSet(p));
  }

  /*--------------------------------------------------------------------------------*/
  /** Set parameter from string representation
   *
   * @param p Parameter_xxx index to check against
   * @param param reference to parameter to set
   * @param name parameter name
   * @param value value
   * @param limit ptr to limit function or NULL
   *
   * @return true if parameter was set from string
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool SetFromValue(Parameter_t p, T1& param, const std::string& name, const std::string& value, T1 (*limit)(const T1& val) = NULL) {
    bool success = false;
    if (name == parameterdescs[p].name)
    {
      T1 val;
      if (Evaluate(value, val))
      {
        param = limit ? (*limit)(val) : val;
        MarkParameterSet(p);
        success = true;
      }
    }
    return success;
  }

  /*--------------------------------------------------------------------------------*/
  /** Set parameter from string representation (with type conversion)
   *
   * @param p Parameter_xxx index to check against
   * @param param reference to parameter to set
   * @param name parameter name
   * @param value value
   * @param limit ptr to limit function or NULL
   *
   * @return true if parameter was set from string
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1,typename T2>
  bool SetFromValueConv(Parameter_t p, T1& param, const std::string& name, const std::string& value, T1 (*limit)(const T2& val) = NULL) {
    bool success = false;
    if (name == parameterdescs[p].name)
    {
      T2 val;
      if (Evaluate(value, val))
      {
        param = limit ? (*limit)(val) : val;
        MarkParameterSet(p);
        success = true;
      }
    }
    return success;
  }

  /*--------------------------------------------------------------------------------*/
  /** Get parameter string representation
   *
   * @param p Parameter_xxx index to check against
   * @param param reference to parameter to get
   * @param name parameter name
   * @param value value
   *
   * @return true if string was set
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool GetToValue(Parameter_t p, const T1& param, const std::string& name, std::string& value) const {
    bool success = false;
    if (name == parameterdescs[p].name)
    {
      value = StringFrom(param);
      success = true;
    }
    return success;
  }

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter to zero by name
   *
   * @param p Parameter_xxx index to check against
   * @param param reference to parameter to reset
   * @param name parameter name
   *
   * @return true if parameter was reset
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool ResetValue(Parameter_t p, T1& param, const std::string& name) {
    bool success = false;
    if (name == parameterdescs[p].name)
    {
      ResetParameter<>(p, param);
      success = true;
    }
    return success;
  }


  /*--------------------------------------------------------------------------------*/
  /** Reset parameter to specified value by name
   *
   * @param p Parameter_xxx index to check against
   * @param param reference to parameter to reset
   * @param name parameter name
   * @param val reset value
   *
   * @return true if parameter was reset
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1,typename T2>
  bool ResetValue(Parameter_t p, T1& param, const std::string& name, const T2& val = T2()) {
    bool success = false;
    if (name == parameterdescs[p].name)
    {
      ResetParameter<>(p, param, val);
      success = true;
    }
    return success;
  }
  
#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Set parameter from JSON
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 intermediate type that JSON framework can manage
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val intermediate value as read from JSON
   * @param obj JSON object
   * @param reset true to reset parameter to its default if it is not found in the JSON object
   * @param defval reset value if reset = true
   * @param limit optional function ptr to a limiting/converting function that will convert the JSON value to a usable parameter value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetFromJSON(Parameter_t p, T1& param, T2& val, const json_spirit::mObject& obj, bool reset = false, const T1& defval = T1(), T1 (*limit)(const T2& val) = NULL)
  {
    json_spirit::mObject::const_iterator it;
    // try and find named item in object
    if ((it = obj.find(parameterdescs[p].name)) != obj.end())
    {
      // read value from JSON into intermediate value
      bbcat::FromJSON(it->second, val);
      // use intermediate value to set parameter
      SetParameter<>(p, param, val, limit);
    }
    // if not found, reset parameter to default
    else if (reset) ResetParameter<>(p, param, defval);
  }    

  /*--------------------------------------------------------------------------------*/
  /** Set JSON from parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter value
   * @param obj JSON object
   * @param force true to force parameters to be set, even if they have not been set
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  void SetToJSON(Parameter_t p, const T1& param, json_spirit::mObject& obj, bool force = false) const
  {
    if (force || IsParameterSet(p)) obj[parameterdescs[p].name] = bbcat::ToJSON(param);
  }    

  /*--------------------------------------------------------------------------------*/
  /** Set JSON from parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of JSON storage
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter value
   * @param obj JSON object
   * @param force true to force parameters to be set, even if they have not been set
   * @param convert function to convert T1 to T2
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetToJSON(Parameter_t p, const T1& param, json_spirit::mObject& obj, bool force, T2 (*convert)(const T1& val)) const
  {
    if (force || IsParameterSet(p)) obj[parameterdescs[p].name] = bbcat::ToJSON((*convert)(param));
  }    
#endif

  template<typename T1>
  void CopyIfSet(const AudioObjectParameters& obj, Parameter_t p, T1& dst, const T1& src) {
    if (obj.IsParameterSet(p))
    {
      dst = src;
      MarkParameterSet(p);
    }
  }
      
  /*--------------------------------------------------------------------------------*/
  /** Mark parameter p as being set
   */
  /*--------------------------------------------------------------------------------*/
  void MarkParameterSet(Parameter_t p)   {setbitmap |=  (1U << p);}

  /*--------------------------------------------------------------------------------*/
  /** Mark parameter p as being *not* set (reset)
   */
  /*--------------------------------------------------------------------------------*/
  void MarkParameterReset(Parameter_t p) {setbitmap &= ~(1U << p);}

  /*--------------------------------------------------------------------------------*/
  /** Return whether parameter p has been set
   */
  /*--------------------------------------------------------------------------------*/
  bool IsParameterSet(Parameter_t p) const {return ((setbitmap & (1U << p)) != 0);}
  
  /*--------------------------------------------------------------------------------*/
  /** Structure of simple data type items
   *
   * @note must contain POD types ONLY!  Is zeroed by code hence no objects can be used.
   *
   * @note the ORDER of the entries is to aid structure packing as much as possible (big to small) because
   * the size of this structure is a massive influence on the size of block formats!
   */
  /*--------------------------------------------------------------------------------*/
  typedef struct {
    uint64_t duration;
    uint64_t interpolationtime;
    double   gain;
    float    width;
    float    height;
    float    depth;
    float    diffuseness;
    float    delay;
    float    divergenceazimuth;
    float    divergencebalance;
	float    channellockmaxdistance;
    uint_t   channel;
    uint8_t  importance;
    uint8_t  dialogue;
    uint8_t  channellock;
    uint8_t  interact;
    uint8_t  interpolate;
    uint8_t  onscreen;
  } VALUES;
      
protected:
  Position     position;
  VALUES       values;
  uint_t       setbitmap;                           // bitmap of values that (good up to 32 items)
  ParameterSet othervalues;                         // additional, arbitrary parameters
  ExcludedZone *excludedZones;
  
  static const PARAMETERDESC parameterdescs[Parameter_count];
};

BBC_AUDIOTOOLBOX_END

#endif
