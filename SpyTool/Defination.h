

typedef enum  type_DEFINE_LIST
{
  TYPE_ALL  = 0,
  TYPE_URL,
  TYPE_DOC,
  TYPE_EXE,
  TYPE_LNK,   //Shortcut
  TYPE_DIR, 
  TYPE_END
} TARGET_TYPE;

typedef struct LINK_TAB
{
  TCHAR  szType[20]; //Web-Site, Documents, Executable, Shortcut, Directory
  TCHAR  szName[50]; //Unique ID
  TCHAR  szLinkage[MAX_PATH]; //Linkage of target 
  TCHAR  szComments[200];  //Used as comments:
  TCHAR  szUSER[32]; //User name - optional
  TCHAR  szPSW[24];  // Password - optional
  TCHAR  szTime[24]; // Time of creation or latest access/update
  TCHAR  szCategory[20]; //Category - Working, Fun, Private
   
} LINK_SET, * LPLINK ;

//#define URL_SET LINK_SET


typedef struct DOC_TAB
{
  TARGET_TYPE   iTargetType;
  TCHAR  szFilePathName[MAX_PATH];
  TCHAR  szDocType[10]; //xls, doc, docx etc

} DOC_SET, * LPDOC ;
