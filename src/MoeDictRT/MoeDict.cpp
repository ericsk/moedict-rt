#include "pch.h"
#include "MoeDict.h"
#include "MoeDictImpl.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace MoeDictRT;
using namespace MoeDictRT::Details;

// Constructor
// Initialize the implementation instance with the dict path
MoeDict::MoeDict(String^ dbPath) 
  : _impl(new MoeDictImpl(dbPath))
{

}

MoeDict::~MoeDict()
{
}

IAsyncOperation<IMap<String^, String^>^>^
  MoeDict::LookupRadicalAsync(String^ word)
{
  // pass the arguments to the implementation instance
  return _impl->LookupRadicalAsync(word);
}