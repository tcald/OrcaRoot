// ORVDataDecoder.cc

#include "ORVDataDecoder.hh"
#include "ORDecoderDictionary.hh"
#include "ORUtils.hh"
#include "ORLogger.hh"
#include <iomanip>
#include <sstream>
using ORUtils::BitConcat;

void ORVDataDecoder::Swap(UInt_t* dataRecord)
{
  for(size_t i=1; i<LengthOf(dataRecord);i++) {
    ORUtils::Swap(dataRecord[i]);
  }
}

void ORVDataDecoder::DumpHex(UInt_t* dataRecord, UInt_t rowLength)
{
  if(rowLength == 0) rowLength = 8;
  std::stringstream str;
  str << "DumpHex for record from DataId " 
      << DataIdOf(dataRecord) << " (= 0x" 
      << std::hex << DataIdOf(dataRecord) << ")";
  for(size_t i=0; i<LengthOf(dataRecord); i++) {
    if(i%rowLength == 0) str << "\n" << std::dec << std::setfill(' ') << std::setw(5) << i << ": ";
    else str << ' ';
    str << std::hex << std::setfill('0') << std::setw(8) << dataRecord[i];
  }
  ORLog(kRoutine) << str.str() << std::endl;
}

const ORVDictValue* ORVDataDecoder::GetValueFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
  if (!fDecoderDictionary) return NULL;
  const ORDictionary* dict = 
    fDecoderDictionary->GetRecordDictWithCrateAndCard(crate, card);
  if (!dict) return NULL;
  return dict->LookUp(key);
}

const ORDictValueA* ORVDataDecoder::GetArrayFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
    return (dynamic_cast<const ORDictValueA*>(GetValueFromKey(key, crate, card)));
}

std::string ORVDataDecoder::GetStringValueFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
  const ORDictValueS* val = 
    dynamic_cast<const ORDictValueS*>(GetValueFromKey(key, crate, card));
  if (!val) {
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return "";
  }
  return val->GetS();
}

Int_t ORVDataDecoder::GetIntValueFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
  const ORDictValueI* val = 
    dynamic_cast<const ORDictValueI*>(GetValueFromKey(key, crate, card));
  if (!val) { 
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return 0;
  }
  return val->GetI();
}

Double_t ORVDataDecoder::GetRealValueFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
  const ORDictValueR* val = 
    dynamic_cast<const ORDictValueR*>(GetValueFromKey(key, crate, card));
  if (!val) { 
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return 0.;
  }
  return val->GetR();
}

Bool_t ORVDataDecoder::GetBoolValueFromKey(std::string key, UInt_t crate,
  UInt_t card)
{
  const ORDictValueB* val = 
    dynamic_cast<const ORDictValueB*>(GetValueFromKey(key, crate, card));
  if (!val) { 
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return false;
  }
  return val->GetB();
}

std::string ORVDataDecoder::GetStringValueFromKeyArray(std::string key, 
  UInt_t crate, UInt_t card, size_t index)
{
  const ORDictValueA* array = GetArrayFromKey(key, crate, card);
  if (!array) {
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return "";
  }
  if (index >= array->GetNValues()) {
    ORLog(kWarning) << "index " << index << " out of range for crate " << crate << ", card " << card << ", key " << key << std::endl; 
    return "";
  } 
  const ORDictValueS* val = 
    dynamic_cast<const ORDictValueS*>(array->At(index));
  if (!val) {
    ORLog(kWarning) << key << " for crate " << crate << ", card " << card << ", index " << index << " is not a string!" << std::endl;
    return "";
  }
  return val->GetS();
}

Int_t ORVDataDecoder::GetIntValueFromKeyArray(std::string key, 
  UInt_t crate, UInt_t card, size_t index)
{
  const ORDictValueA* array = GetArrayFromKey(key, crate, card);
  if (!array) {
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return 0;
  }
  if (index >= array->GetNValues()) {
    ORLog(kWarning) << "index " << index << " out of range for crate " << crate << ", card " << card << ", key " << key << std::endl; 
    return 0;
  } 
  const ORDictValueI* val = 
    dynamic_cast<const ORDictValueI*>(array->At(index));
  if (!val) {
    ORLog(kWarning) << key << " for crate " << crate << ", card " << card << ", index " << index << " is not an int!" << std::endl;
    return 0;
  }
  return val->GetI();
}

Double_t ORVDataDecoder::GetRealValueFromKeyArray(std::string key, 
  UInt_t crate, UInt_t card, size_t index)
{
  const ORDictValueA* array = GetArrayFromKey(key, crate, card);
  if (!array) {
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return 0.;
  }
  if (index >= array->GetNValues()) {
    ORLog(kWarning) << "index " << index << " out of range for crate " << crate << ", card " << card << ", key " << key << std::endl; 
    return 0.;
  } 
  const ORDictValueR* val = 
    dynamic_cast<const ORDictValueR*>(array->At(index));
  if (!val) {
    ORLog(kWarning) << key << " for crate " << crate << ", card " << card << ", index " << index << " is not a float!" << std::endl;
    return 0.;
  }
  return val->GetR();
}

Bool_t ORVDataDecoder::GetBoolValueFromKeyArray(std::string key, 
  UInt_t crate, UInt_t card, size_t index)
 {
  const ORDictValueA* array = GetArrayFromKey(key, crate, card);
  if (!array) {
    ORLog(kWarning) << key << " not found for crate: " << crate 
      << ", card: " << card << std::endl;
    return false;
  }
  if (index >= array->GetNValues()) {
    ORLog(kWarning) << "index " << index << " out of range for crate " << crate << ", card " << card << ", key " << key << std::endl; 
    return false;
  } 
  const ORDictValueB* val = 
    dynamic_cast<const ORDictValueB*>(array->At(index));
  if (!val) {
    ORLog(kWarning) << key << " for crate " << crate << ", card " << card << ", index " << index << " is not a bool!" << std::endl;
    return false;
  }
  return val->GetB();
}
