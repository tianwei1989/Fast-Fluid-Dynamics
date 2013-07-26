#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "data_structure.h"
#include "utility.h" 
#define BUF_SIZE 256
#define BUF_DATA_SIZE 2560

TCHAR ffdDataName[] = TEXT("FFDDataMappingObject");
TCHAR otherDataName[] = TEXT("ModelicaDataMappingObject");

/******************************************************************************
 Write shared data to the shared memory 
******************************************************************************/
int write_to_shared_memory(ffdSharedData *ffdData)
{
  HANDLE DataMapFile;
  LPCTSTR DataBuf;
  char msg[100];

  /*---------------------------------------------------------------------------
  | Open the named file mapping objects
  ---------------------------------------------------------------------------*/
  DataMapFile = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,    // read/write access
                    FALSE,           // do not inherit the name
                    ffdDataName);    // name of mapping object for FFD data

  // Send warning if can not open shared memory
  if(DataMapFile==NULL)
  {
    sprintf(msg, "cosimulation.c: Could not open FFD data file mapping object. Error code %d", GetLastError());
    ffd_log("cosimulation.c: Could not open FFD data file mapping object.", FFD_ERROR);
    return 1;
  }
 
  /*---------------------------------------------------------------------------
  | Mps a view of a file mapping into the address space of a calling process
  ---------------------------------------------------------------------------*/
  DataBuf = (LPTSTR) MapViewOfFile(DataMapFile,   // handle to map object
                      FILE_MAP_ALL_ACCESS, // read/write permission
                      0,
                      0,
                      BUF_DATA_SIZE);

  if(DataBuf == NULL)
  {
    sprintf(msg, "cosimulation.c: Could not map view of FFD data file. Error code %d", GetLastError());
    CloseHandle(DataMapFile);
    return 1;
  }
  // Copy a block of memory from ffdData to ffdDaraBuf
  CopyMemory((PVOID)DataBuf, ffdData, sizeof(ffdSharedData));

  UnmapViewOfFile(DataBuf);
  CloseHandle(DataMapFile);

  return 0;
} // End of write_to_shared_memory()

/******************************************************************************
 Read shared data from the shared memory 
******************************************************************************/
int read_from_shared_memory(otherSharedData *otherData)
{
  HANDLE DataMapFile;
  char msg[100];
  otherSharedData *data;

  /*---------------------------------------------------------------------------
  | Open the named file mapping objects
  ---------------------------------------------------------------------------*/
  DataMapFile = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,    // read/write access
                    FALSE,           // do not inherit the name
                    otherDataName);    // name of mapping object for FFD data

  // Send warning if can not open shared memory
  if(DataMapFile==NULL)
  {
    sprintf(msg, "cosimulation.c: Could not open other data file mapping object. Error code %d", GetLastError());
    ffd_log("cosimulation.c: Could not open other data file mapping object.", FFD_ERROR);
    return 1;
  }
 
  /*---------------------------------------------------------------------------
  | Mps a view of a file mapping into the address space of a calling process
  ---------------------------------------------------------------------------*/
  data = (otherSharedData *) MapViewOfFile(DataMapFile,   // handle to map object
                      FILE_MAP_ALL_ACCESS, // read/write permission
                      0,
                      0,
                      BUF_DATA_SIZE);

  if(data == NULL)
  {
    sprintf(msg, "cosimulation.c: Could not map view of other data file. Error code %d", GetLastError());
    CloseHandle(DataMapFile);
    return 1;
  }

  otherData->arr[0] = data->arr[0];
  otherData->arr[1] = data->arr[1];
  otherData->arr[2] = data->arr[2];
  otherData->command = data->command;
  otherData->number = data->number;
  printf("%s\n", data->message);
  strcpy(otherData->message, data->message);

  UnmapViewOfFile(data);
  CloseHandle(DataMapFile);

  return 0;
} // End of read_from_shared_memory()