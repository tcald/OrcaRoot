// ORKatrinFLTWaveformDecoder.cc

#include "ORKatrinFLTWaveformDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORKatrinFLTWaveformDecoder::ORKatrinFLTWaveformDecoder() { fDataRecord = NULL; fWaveformLength = -1; }

bool ORKatrinFLTWaveformDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  fWaveformLength = (  LengthOf(fDataRecord) / (kWaveformLength/2)  )  * 1024;

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  // remarks for Till: LengthOf(...) is from ORVDataDecoder and is the length extracted from data record -tb-
  if(!IsValid() || LengthOf(fDataRecord) != kBufHeadLen + GetWaveformLen()/2) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    ORLog(kDebug) << "LengthOf(data record) : " << LengthOf(fDataRecord)
      << " kBufHeadLen + GetWaveformLen()/2: " << kBufHeadLen + GetWaveformLen()/2 << std::endl;
    fDataRecord = NULL;
    fWaveformLength = -1;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}

bool ORKatrinFLTWaveformDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void ORKatrinFLTWaveformDecoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=2;i<kBufHeadLen; i++)
    {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}

size_t ORKatrinFLTWaveformDecoder::CopyWaveformData( UShort_t* waveform, 
                                                     size_t len )
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    //bin swapping handling changed 2008-11-18 (svn rev >=1354)  ml, -tb-
    waveform[i] = (waveformData[i/2] & 0xFFFF);  
    waveform[i+1] = ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
  }
  return len;
}

size_t ORKatrinFLTWaveformDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    //bin swapping handling changed 2008-11-18 (svn rev >=1354)  ml, -tb-
    waveform[i] = (double) (waveformData[i/2] & 0xFFFF);  
    waveform[i+1] = (double) ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
  }
  return len;
}
//debugging: *************************************************************************


void ORKatrinFLTWaveformDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  ORLog(kDebug) << std::endl << std::endl << "ORKatrinFLTWaveformDecoder::Dump():" << std::endl ;
  if(!SetDataRecord(dataRecord)) return; 
  ORLog(kDebug) 
    << "  Header functions: " << std::endl
    << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
    << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << std::endl
    << "    The Sec is " << GetSec() << std::endl
    << "    The SubSec is " << GetSubSec() << std::endl
    << "    The channel is " << GetChannel() << std::endl
    << "    The channel map is " << GetChannelMap() << std::endl
    << "    Energy: " << GetEnergy() << std::endl  
    << "    The ResetSec is " << GetResetSec() << std::endl        // -tb- 2008-02-12
    << "    The ResetSubSec is " << GetResetSubSec() << std::endl  // -tb- 2008-02-12
    << "    The waveform data has " << GetWaveformLen() 
      << " (32-bit) words" << std::endl;
}
