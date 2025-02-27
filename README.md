# ft_irc

![C++](https://img.shields.io/badge/C++-98-blue.svg)
![Network](https://img.shields.io/badge/Network-Socket%20Programming-green.svg)
![I/O Multiplexing](https://img.shields.io/badge/I%2FO-Multiplexing-orange.svg)

RFC 표준을 준수한 Internet Relay Chat (IRC) 서버 구현 프로젝트입니다. C++98과 I/O 멀티플렉싱을 사용하여 효율적이고 안정적인 채팅 서버를 구현했습니다.

## Table of Contents
- [주요 기능](#주요-기능)
- [구현 상세](#구현-상세)
- [시작하기](#시작하기)
- [사용 예시](#사용-예시)
- [기술 스택](#기술-스택)
- [프로젝트 구조](#프로젝트-구조)
- [에러 처리](#에러-처리)
- [참고 문서](#참고-문서)

## 주요 기능

### IRC 프로토콜 핵심 구현
- RFC 1459, 2812 표준 준수
- 실제 IRC 클라이언트와 완벽한 호환성
- 다중 클라이언트 동시 접속 지원

### 이벤트 기반 I/O 멀티플렉싱
- kqueue를 활용한 효율적인 이벤트 관리
- 논블로킹 I/O 작업
- 비동기 메시지 처리
- 안정적인 동시 연결 처리

### IRC 명령어 지원
- **인증 & 등록**
  - PASS: 서버 비밀번호 인증
  - NICK: 닉네임 설정
  - USER: 사용자 등록

- **채널 관리**
  - JOIN: 채널 참여
  - KICK: 사용자 추방
  - INVITE: 채널 초대
  - TOPIC: 채널 주제 설정
  - MODE: 채널/사용자 모드 설정

- **메시지 & 통신**
  - PRIVMSG: 개인/채널 메시지 전송
  - PING/PONG: 연결 상태 확인

## 구현 상세

### 채널 관리 시스템
- 동적 채널 생성 및 삭제
- 채널 운영자 권한 관리
- 채널별 사용자 목록 관리
- 채널 모드 설정 기능

### 사용자 관리
- 고유 닉네임 시스템
- 사용자 인증 프로세스
- 상태 관리 및 모니터링
- 권한 레벨 시스템

### 메시지 처리
- 메시지 파싱 및 검증
- 명령어 핸들링
- 에러 메시지 반환
- 브로드캐스팅 시스템

## 시작하기

### 요구사항
- C++ 컴파일러 (C++98 지원)
- Make

### 컴파일
```bash
make
```

### 실행
```bash
./ircserv <port> <password>
```

### 매개변수
- **port**: 서버 포트 번호 (0-65535)
- **password**: 서버 접속 비밀번호

### 실행 예시
```bash
./ircserv 6667 serverpassword
```

## 사용 예시

### IRC 클라이언트 연결
```bash
# irssi 사용 예시
/connect localhost 6667 <password> <nickname>

# nc 사용 예시
nc localhost 6667
PASS serverpassword
NICK nickname
USER username hostname servername :realname
```

### 기본 명령어
```bash
# 채널 생성 및 참여
/JOIN #channel

# 메시지 전송
/PRIVMSG #channel :Hello, World!

# 채널 주제 설정
/TOPIC #channel :Welcome to our channel!
```

## 기술 스택

- Language: C++
- Network: Berkeley sockets API
- Event Handling: kqueue (macOS)

## 프로젝트 구조

```
├── Include/           # 헤더 파일
│   ├── Command/      # IRC 명령어 처리 클래스
│   ├── Server.hpp    # 서버 코어 기능
│   ├── Channel.hpp   # 채널 관리
│   └── Client.hpp    # 클라이언트 관리
├── Source/           # 구현 파일
└── Makefile
```

## 에러 처리

### 네트워크 레벨
- 잘못된 포트 번호 검증
- 연결 끊김 감지 및 복구
- SIGPIPE 시그널 처리
- 소켓 에러 처리

### IRC 프로토콜 레벨
- 잘못된 명령어 구문 처리
- 권한 검증
- 중복 닉네임 처리
- 타임아웃 처리

## 참고 문서

### RFC 문서
- [RFC 1459](https://tools.ietf.org/html/rfc1459) - Internet Relay Chat Protocol
- [RFC 2812](https://tools.ietf.org/html/rfc2812) - Internet Relay Chat: Client Protocol

### 기술 문서
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [BSD Sockets Programming Guide](https://docs.freebsd.org/en/books/developers-handbook/sockets/)

## ft_irc를 만든 이유

네트워크 프로그래밍의 기초를 확실히 다지고, 실제 서비스에서 사용되는 프로토콜을 구현해보기 위해 시작한 프로젝트입니다.

- **소켓 프로그래밍**: Berkeley 소켓 API를 직접 다루며 네트워크 프로그래밍의 핵심 개념을 습득했습니다.
- **프로토콜 구현**: IRC 프로토콜 명세를 분석하고 구현하면서 네트워크 프로토콜의 동작 방식을 깊이 이해했습니다.
- **동시성 처리**: I/O 멀티플렉싱을 통해 다중 클라이언트 처리의 효율적인 방법을 학습했습니다.
