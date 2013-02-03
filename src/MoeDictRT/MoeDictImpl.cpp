#include "pch.h"
#include "MoeDictImpl.h"

using namespace std;
using namespace Concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace MoeDictRT;
using namespace MoeDictRT::Details;


template <typename Result>
task<Result> task_from_result(Result result)
{
  return create_task([result]() -> Result { return result; });
}

// Constructor
// <param name="dbPath" type="Platform::String^" desc="The path of the dict db file." />
MoeDictImpl::MoeDictImpl(String^ dbPath)
{
  // open dict
  int rc = sqlite3_open16(dbPath->Data(), &_db);
  if (rc)
  {
    sqlite3_close(_db);
    _db = nullptr;
  }
}

MoeDictImpl::~MoeDictImpl()
{
  if (_db != nullptr)
  {
    sqlite3_close(_db);
  }
}

IAsyncOperation<IMap<String^, String^>^>^
  MoeDictImpl::LookupRadicalAsync(String^ word)
{
  return create_async([this, word](cancellation_token cancellationToken) -> task<IMap<String^, String^>^>
  {
    return LookupRadicalTask(word, cancellationToken)
      .then([this](task<IMap<String^, String^>^> previousTask) -> IMap<String^, String^>^
    {
      try
      {
        return previousTask.get();
      }
      catch (const task_canceled&)
      {
        return nullptr;
      }
    });
  });
}

task<IMap<String^, String^>^> 
  MoeDictImpl::LookupRadicalTask(String^ word, cancellation_token cancellationToken)
{

  int rc;

  auto completionTask = create_task(_completionEvent);

  if (_db != nullptr)
  {
    sqlite3_stmt *stmt;
    wostringstream wss;
    
    wss << L"select radical from entries where title = '" << word->Data() << L"\'";

    auto ss = ref new String(wss.str().data());
    
    rc = sqlite3_prepare16(_db, ss->Data(), -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
      sqlite3_finalize(stmt);
      wss.clear();
      wss << L"Error: " << sqlite3_errmsg(_db);

      _completionEvent.set(make_tuple<HRESULT, String^>(move(E_FAIL), move(ref new String(wss.str().data()))));
    }
    else
    {
      rc = sqlite3_step(stmt);
      if (rc == SQLITE_ROW) {
        auto buf = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 0));
        _completionEvent.set(make_tuple<HRESULT, String^>(move(S_OK), move(ref new String(buf))));
      }
    }
  }
  else
  {

  }

  return completionTask.then([this](tuple<HRESULT, String^> resultTuple)
  {
    auto ret = ref new Map<String^, String^>();

    HRESULT result = std::get<0>(resultTuple);
    if (result == S_OK)
    {
      ret->Insert(ref new String(L"Error"), ref new String(L""));
      ret->Insert(ref new String(L"Radical"), std::get<1>(resultTuple));
    }
    else
    {
      ret->Insert(ref new String(L"Error"), std::get<1>(resultTuple));
      ret->Insert(ref new String(L"Radical"), ref new String(L""));
    }
    return task_from_result<IMap<String^, String^>^>(ret);
  });
}