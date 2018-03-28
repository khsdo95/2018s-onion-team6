#include "./PgpManager.h"
using namespace std;


enum {
    GET_GPG_VERSION = 0,
    IMPORT_PRIVATE_KEY,
    GET_LIST_KEYS
};

char* const  MY_GPG_OPTION[] = {
    "--version",
    "--import",
    "--list-keys"
};



PgpManager::~PgpManager() {}

PgpManager::PgpManager(string passwd) {
    passphrase = passwd;
    if (CheckProperGPG())
    {
        cout << "PROPER_GPG" << endl;              // proper handling needed
    }
    else
    {
        cout << "INVALID_GPG" << endl;
    }

    ImportMyPrivateKey(PRIVATE_KEY);
    //getKeyList();

    //EncryptData("test.txt", "c6140206");
    //cout << "encrypt done" << endl;
    //DecryptData("test.txt.gpg", "decryptedData.txt");
    //cout << "decrypt done" << endl;

}

PgpManager::PgpManager() {
   // PgpManager();
}

int
PgpManager::ImportMyPrivateKey(char*  privKey) {

    string TAG = "ImportMyPrivateKey";
    char* argv[] = {
        GPG_COMMAND, 
        MY_GPG_OPTION[IMPORT_PRIVATE_KEY],
        privKey,
        NULL
    };
    CallLocalGPG(TAG,argv);
    // string handling needed
    //  str
    //  int index = strstr(DisplayedBuf, "key");
    //  strncpy(MY_PUBKEY, DisplayedBuf[index+4], 8);
    //  MY_PUBKEY[8] = '\0';
    //cout << "---------------My PubKey : " << MY_PUBKEY << endl;

}

int
PgpManager::EditKey(char* key){
       string TAG = "EDITKEY";
       char* argv[] = {
        GPG_COMMAND,
        "--edit-key",
        key,
        NULL

       };

       CallLocalGPG(TAG, argv);

}

int 
PgpManager::RecvKey(char* pubKey){

    string TAG = "RecvKey";
    char* argv[] = {
        GPG_COMMAND,
        "--keyserver",
        "keyserver.ubuntu.com",
        "--recv-keys",
        pubKey,
        NULL
    };
    CallLocalGPG(TAG, argv);
}

int 
PgpManager::AddPubkey(char* pubKey){

    // recv & save to userInfo
       RecvKey(pubKey);
       //adding this key to userinfo class is needed

    
}

void
PgpManager::getKeyList(){

    string TAG = "getKeyList";
    char* argv[] = {
        GPG_COMMAND,
        MY_GPG_OPTION[GET_LIST_KEYS],
        NULL
    };
    CallLocalGPG(TAG, argv);
    /*
       printf("Output : (%.*s)\n", nbytes, DisplayedBuf);
     */

}


void
PgpManager::CleanDisplayedBuf() {
    for (int i = 0; i<MAX_DISPLAY_BUF; i++)DisplayedBuf[i] = '\0';
    nbytes = 0;
}

int
PgpManager::DecryptData(char* src, char* dst){
    string TAG = "DECRYPT";
    char* argv[]={
        GPG_COMMAND,
        "--yes",
        "--passphrase",
        MY_PASSPHRASE,
        "-o",
        dst,
        "--decrypt",
        src,
        NULL
    };

    CallLocalGPG(TAG, argv);

}


int
PgpManager::EncryptData(char* srcFilename, char* pubKeyID){

    //gpg  --yes -r c6140206 --encrypt-files test.txt
    string TAG = "ENCRYPT";
    char* argv[]={
        GPG_COMMAND,
        "--yes",
        "-r",
        pubKeyID,
        "--encrypt-files",
        srcFilename,
        //"test.txt",
        NULL
    };
    CallLocalGPG(TAG, argv);
}


    void
PgpManager::CallLocalGPG(string msg, char* const argv[])
{
    DEBUG cout << msg + "start!!" << endl;
    int link[2];
    CleanDisplayedBuf();
    pid_t pid;


    pipe(link);                 // error handling needed
    pid = fork();               // error hadnling needed

    // Pid handling needed
    if (pid == 0) {
        /*
           DEBUG{
           cout << argv[0] << endl;
           cout << argv[1] << endl;;
           cout << argv[2] << endl;
           }
         */


        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);

        execv(GPG_PATH, argv );
    }

    else {

        int status;
        pid_t mChild = wait(&status);
        cout << "----waiting : " << msg << "----" << endl;

        close(link[1]);

        int nbytes = read(link[0], DisplayedBuf, MAX_DISPLAY_BUF);
        DEBUG  printf("Output : (%.*s)\n", nbytes, DisplayedBuf);       // parsing needed

    }

}


void
PgpManager::Authentication(){

    string TAG = "Authentication";
    // auth file path handling needed
   // EncryptData("auth",
}


int
PgpManager::CheckProperGPG() {

    string TAG = "CheckerProperGPG";
    char* argv[] = {
        GPG_COMMAND,
        MY_GPG_OPTION[GET_GPG_VERSION],
        NULL
    };


    CallLocalGPG(TAG, argv);

    char sig[11];
    for (int i = 0; i<10; i++) sig[i] = DisplayedBuf[i];
    sig[10] = '\0';
    string mSig = sig;


    DEBUG printf("Output : (%.*s)\n", MAX_DISPLAY_BUF, DisplayedBuf);
    if (mSig.find("(GnuPG") != string::npos) {

        return PROPER_GPG;
    }


    return INVALID_GPG;


}




