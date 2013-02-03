#pragma once

namespace MoeDictRT
{
  namespace Details
  {
    class MoeDictImpl
    {
    public:
      MoeDictImpl(Platform::String^ dbPath);
      ~MoeDictImpl();

      /// <summary>
      /// Look up the radical of a word
      /// </summary>
      /// <param name="word" type="Platform::String^">The word to be looked up.</param>
      /// <returns type="Windows::Foundation::IAsyncOperation<Platform::String^>^" />
      Windows::Foundation::IAsyncOperation<
      Windows::Foundation::Collections::IMap<
        Platform::String^, 
        Platform::String^
      >^>^
        LookupRadicalAsync(Platform::String^ word);

    private:
      // the pointer to the sqlite db instance.
      sqlite3* _db;

      // the event instance that indicates if the operation has been done.
      Concurrency::task_completion_event<std::tuple<HRESULT, Platform::String^>> 
        _completionEvent;

      // the actual task of the lookup.
      Concurrency::task<Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^>
        LookupRadicalTask(Platform::String^ word, Concurrency::cancellation_token cancellationToken);

    };
  }
}