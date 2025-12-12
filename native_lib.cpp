#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include "BS_thread_pool.hpp"

namespace fs = std::filesystem;
using str = std::string;

QUEUE_STACK = 4; //스레드당 큐의 개수 Q1(사용위치)



// ============================================================
// 1. [Compile-Time] 문자열 해시 함수
//    (switch-case의 case 라벨에 사용)
// ============================================================
// FNV-1a 32bit Hash Algorithm
constexpr uint32_t hash_compile_time(const char* string) {
    uint32_t hash = 0x811c9dc5; // FNV Offset Basis
    
    while (*string) {
        hash ^= (unsigned char)*string++;
        hash *= 0x01000193; // FNV Prime
    }
    return hash;
}

// 편의성: 사용자 정의 리터럴 (Syntactic Sugar)
// 사용법: "AES"_hash 라고 쓰면 바로 숫자로 변환됨
constexpr uint32_t operator"" _hash(const char* string, size_t) {
    return hash_compile_time(string);
}


// ============================================================
// 2. [Runtime] 문자열 해시 함수
//    (실제 변수로 들어오는 string을 처리)
// ============================================================
uint32_t hash_runtime(const str& string) {
    // 컴파일 타임 함수와 동일한 로직을 호출하여 결과값 일치 보장
    return hash_compile_time(string.c_str());
}

// -----------------------------------------------------------
// [상수 정의] 플래그 비트마스크
// -----------------------------------------------------------
// 1번째 비트: MAC이 유효한 값이면 1, 아니면 0
const uint8_t FLAG_HAS_MAC = 0x01; 

// (예시) 나중에 추가 가능한 확장성
// const uint8_t FLAG_COMPRESSED = 0x02; 
// const uint8_t FLAG_ENCODED   = 0x04;

#pragma pack(push, 1) 
struct Header {
    // [1. 보안 파라미터] (32 * 3 = 96 bytes)
    uint8_t salt[32];       // Key Derivation Salt
    uint8_t iv[32];         // IV / Tweak
    uint8_t mac[32];        // HMAC / Tag / Hash 저장 공간

    // [2. 실행 파라미터] (4 bytes)
    uint32_t chunkSize;     // Lion/병렬처리를 위한 청크 크기

    // [3. 식별자 & 옵션] (1 * 3 = 3 bytes)
    uint8_t algo;           // 알고리즘 ID
    uint8_t mode;           // 모드 ID
    uint8_t flags;          // [NEW] 옵션 플래그 (MAC 유무 등 표시)

    // [4. 시그니처] (8 bytes)
    char magic[8];          // "MY_CRYPT"
};
#pragma pack(pop)

//111byte size header

int8_t get_algo(const str& algo){
    switch (hash_runtime(algo)){
        case "AES-256"_hash:
            return 1;
        case "Twofish"_hash:
            return 2;
        case "Threefish"_hash:
            return 3;
        case "Lion-light"_hash:
            return 4;
        case "Lion-standard"_hash:
            return 5;
        case "Lion-heavy"_hash:
            return 6;
        case "Lion-keter"_hash:
            return 7;
        default:
            return -1;
    }
}

uint8_t get_mod(const str& mod){
    switch (hash_runtime(mod)){
        case "XTS"_hash:
            return 1;
        case"CTR"_hash:
            return 2;
        default:
            return -1;
    }
}

struct FIlehandle {
    int fd;
    size_t size;
    off_t offset;
    int prot;
    int flags;
} // mmap용 파일 핸들 구조체(기본형)

void get_handle(const str& filepath, FIlehandle& handle)
{
    int fd = open(filepath.c_str(), O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    handle.fd = fd;
    handle.size = st.st_size;
    handle.offset = 0;
    handle.prot = PROT_READ;
    handle.flags = MAP_SHARED;
}

uint8_t* map_file(const int work, const FIlehandle& handle, off_t offset, int prot, int flags)
{
    size_t size;
    switch (work){
        case 1: //원본 읽기
             size = handle.size;
             break;
        case 2: //암호화 파일 쓰기
            size = handle.size + sizeof(Header);
            break;
        case 3: //암호화 파일 읽기
            size = handle.size;
            break;
        case 4: //복호화 파일 쓰기
            size = handle.size - sizeof(Header);
            break;
    }
    return static_cast<uint8_t*>(mmap(NULL, size, prot, flags, handle.fd, offset));
}


void buffered_ctr_worker(const uint8_t* plainfile, const uint8_t* tempfile, const int8_t algorithm, const str& hexkey, const str& hexiv, int start, int chunksize, int chunknum, int threadcount){
    
    auto key = Botan::hex_decode_locked(hexkey);
    auto iv = Botan::hex_decode_locked(hexiv);
    str algo;
    
    switch(algorithm){
        case 1:
            algo = "CTR-BE(AES-256)"
            break;
        case 2:
            algo = "CTR-BE(Twofish)"
            break;
        case 3:
            algo = "CTR-BE(Threefish-512)"
            break;
    }
            
    BS::thread_pool pool(threadcount);
   
    std::counting_semaphore<> gate(threadcount*QUEUE_STACK); //Q1

}








int encrypt(const str& filepath, const str& writepath, const str& pbkdf, const str& algorithm, const str& mode, const str& key, const str& iv, int threadcount, bool mac){
    
    
}
