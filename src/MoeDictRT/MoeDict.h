#pragma once

namespace MoeDictRT
{
  namespace Details
  {
    class MoeDictImpl;
  }

  // Represents the result of looking up the radical
  public value struct LookupRadicalResultValue
  {
    Platform::String^ Error;
    Platform::String^ Radical;
  };

  public ref class LookupRadicalResult sealed
  {
  private:
    LookupRadicalResultValue _value;
  public:
    LookupRadicalResult()
    {
      _value.Error = nullptr;
      _value.Radical = nullptr;
    }

    property Platform::String^ Error 
    {
      Platform::String^ get() { return _value.Error; }
      void set(Platform::String^ error) { _value.Error = error; }
    }

    property Platform::String^ Radical
    {
      Platform::String^ get() { return _value.Radical; }
      void set(Platform::String^ radical) { _value.Radical = radical; }
    }
  };

  public ref class MoeDict sealed
  {
  public:
    MoeDict(Platform::String^ dbPath);

    /// <summary>
    /// Look up the radical of a word
    /// </summary>
    /// <param name="word" type="String^">The word to be looked up.</param>
    /// <returns type="LookupRadicalResult" />
    Windows::Foundation::IAsyncOperation<LookupRadicalResult^>^
      LookupRadicalAsync(Platform::String^ word);
    

  private:
    ~MoeDict();

    // the implementation of MoeDict manipulations
    std::unique_ptr<Details::MoeDictImpl> _impl;
  };
}