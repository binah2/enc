#ifndef NATIVE_LIB_H
#define NATIVE_LIB_H

#include <cstdint>

// =============================================================================
// 1. Export Macros (OS 호환성 처리)
// =============================================================================
// 윈도우(DLL)와 리눅스/안드로이드(.so)의 심볼 노출 방식 차이를 매크로로 흡수합니다.
#if defined(_WIN32)
    #define NATIVE_EXPORT __declspec(dllexport)
#else
    #define NATIVE_EXPORT __attribute__((visibility("default")))
#endif

// =============================================================================
// 2. C Interface (JNA / FFI 호환)
// =============================================================================
extern "C" {

    /**
     * @brief 암호화/복호화 작업 설정을 담는 구조체
     * * [Java JNA 매핑 가이드]
     * - const char** -> String[]
     * - const char* -> String
     * - uint8_t* -> Pointer (Memory)
     * - int32_t      -> int
     * - int8_t       -> byte
     */
    struct InitOptions {
        // ---------------------------------------------------------------------
        // [파일 입출력] - 다중 파일 처리 지원
        // ---------------------------------------------------------------------
        // Java/Dart에서 문자열 배열을 넘기면 됩니다.
        const char** filePaths;     // 원본 파일 경로들의 배열 (Null-terminated Strings)
        int32_t fileCount;          // 처리할 파일의 총 개수
        
        const char* outputDir;      // 결과물 저장 폴더 (NULL이면 원본 경로에 저장)

        // ---------------------------------------------------------------------
        // [알고리즘 및 모드 선택]
        // Frozen Map에 정의된 문자열과 정확히 일치해야 합니다.
        // ---------------------------------------------------------------------
        // 예: "AES-256", "ChaCha20", "Lion-Speed", "Lion-Leviathan"
        const char* algorithm;      

        // 일반 모드일 때만 사용 (Lion 프리셋 사용 시 무시됨)
        // 예: "GCM", "CTR", "EAX", "OCB", "XTS"
        const char* mode;           

        // ---------------------------------------------------------------------
        // [키 및 보안 파라미터]
        // ---------------------------------------------------------------------
        // 사용자가 입력한 키 (길이 무관). 내부적으로 HKDF를 통해 필요한 만큼 확장됨.
        uint8_t* keyPtr;            
        int32_t keyLen;             

        // 초기화 벡터 (Nonce). Lion 모드에서는 내부적으로 자동 생성될 수도 있음.
        uint8_t* ivPtr;             
        int32_t ivLen;              

        // ---------------------------------------------------------------------
        // [AEAD 전용 파라미터 (GCM, EAX, OCB)]
        // ---------------------------------------------------------------------
        // 추가 인증 데이터 (Associated Data) - 암호화되지 않지만 위변조 검증용
        uint8_t* aadPtr;            
        int32_t aadLen;             
        
        // 인증 태그 길이 (보통 16 바이트)
        int32_t tagLen;             

        // ---------------------------------------------------------------------
        // [실행 제어 옵션]
        // ---------------------------------------------------------------------
        // 1 = 싱글 스레드 (단일 청크)
        // N > 1 = 멀티 스레드 (파일을 N개 청크로 분할 병렬 처리)
        int32_t threadCount;        
        
        // 1 = 암호화 (Encrypt), 0 = 복호화 (Decrypt)
        int8_t isEncrypt;           
    };

    /**
     * @brief 암호화 작업을 수행하는 메인 함수
     * * @param options 설정값이 담긴 구조체 포인터
     * @return int32_t 상태 코드
     * 0: 성공 (Success)
     * -1: 잘못된 옵션 (Invalid Options / Null Pointer)
     * -2: 알고리즘 오류 (Unknown Algorithm/Mode)
     * -3: 파일 입출력 오류 (I/O Error)
     * -4: 암호화/복호화 실패 (Crypto Failure / Integrity Check Failed)
     */
    NATIVE_EXPORT int32_t execute_crypto_job(InitOptions* options);

    /**
     * @brief (유틸리티) 현재 라이브러리 버전 반환
     */
    NATIVE_EXPORT int32_t get_library_version();
}

#endif // NATIVE_LIB_H
