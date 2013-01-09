#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

#define PC_SUCCESS          0
#define PC_ERROR            -1

#define TRUE                1
#define FALSE               0

/**** Compilation Environment settings ****/
#define CROSS_COMPILE       0

/**** Spotify Envrironment settings ****/
#define REMEMBER_LOGIN      0
#define PREFERRED_BITRATE   0   //0 = 160k , 1 = 320k , 2 = 96k

#if CROSS_COMPILE
#define CREDENTIAL_STORAGE  "/home/pi/cred.txt"
#define SETTINGS_LOCATION   "/home/pi/tmp"
#define CACHE_LOCATION      "/home/pi/tmp"
#else
#define CREDENTIAL_STORAGE  "/home/raphio/cred.txt"
#define SETTINGS_LOCATION   "/home/raphio/tmp"
#define CACHE_LOCATION      "/home/raphio/tmp"
#endif

#define SPOTIFY_USER_AGENT  "wmusic"


/**** System Environment settings ****/
#define SET_ENV             0
#define ESSID               "wMusic"
#define KEY                 "azert"

pthread_mutex_t mutexSession;

enum accountManagerState
{
    INVALID_LOGIN_INFO,
    NOT_PRENIUM,
    CONNECTION_ERROR,
    CONNECTION_OK
};

enum searchManagerState
{
    SEARCH_ERROR,
    SEARCH_OK
};

typedef struct wav_hdr
{
    char        RIFF[4];        // RIFF Header
    int         ChunkSize;      // RIFF Chunk Size
    char        WAVE[4];        // WAVE Header
    char        fmt[4];         // FMT header
    int         Subchunk1Size;  // Size of the fmt chunk
    short int   AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    short int   NumOfChan;      // Number of channels 1=Mono 2=Stereo
    int         SamplesPerSec;  // Sampling Frequency in Hz
    int         bytesPerSec;    // bytes per second */
    short int   blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    short int   bitsPerSample;  // Number of bits per sample
    char        Subchunk2ID[4]; // "data"  string
    int         Subchunk2Size;  // Sampled data length
}wavHeader_t;


#endif // TYPES_H
