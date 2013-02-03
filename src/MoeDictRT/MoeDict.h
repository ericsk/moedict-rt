#pragma once

namespace MoeDictRT
{
  namespace Details
  {
    class MoeDictImpl;
  }

  public ref class MoeDict sealed
  {
  public:
    MoeDict(Platform::String^ dbPath);

    /// <summary>
    /// Look up the radical of a word
    /// </summary>
    /// <param name="word" type="String^">The word to be looked up.</param>
    /// <returns type="IAsyncOperation<IMap<String^, String^>^>^" />
    Windows::Foundation::IAsyncOperation<
      Windows::Foundation::Collections::IMap<
        Platform::String^, 
        Platform::String^
      >^>^
      LookupRadicalAsync(Platform::String^ word);

  private:
    ~MoeDict();

    // the implementation of MoeDict manipulations
    std::unique_ptr<Details::MoeDictImpl> _impl;
  };
}