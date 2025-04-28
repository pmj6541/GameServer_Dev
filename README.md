# 🎮 GameServer_Dev_Study

C++ 기반으로 게임 서버 개발자가 되기 위해 학습한 실전 프로젝트 모음입니다.  
TCP 통신부터 멀티 클라이언트 서버, 패킷 구조화, 세션 및 룸 관리, 간단한 MMO 서버까지 단계별로 구현하며 성장하는 과정을 기록합니다.

---

## 📚 학습 목표
- C++ 네트워크 프로그래밍 기초(TCP, UDP)
- 멀티 클라이언트 서버 구조 이해 및 구현
- 패킷 기반 통신 프로토콜 설계
- 세션/방(Room) 시스템 구축
- 간단한 MMO 서버 제작

---

## 📂 프로젝트 구조

| 단계 | 설명 |
|:--|:--|
| `01_Simple_TCP_Connection/` | TCP 소켓 기본 통신 구축 (단일 클라이언트) |
| `02_MultiClient_Server/` | 다중 클라이언트 지원 서버 (std::thread 기반) |
| `02-0_NonBlocking_Socket_Server/` | 논블로킹 소켓 및 select() 기반 서버 실습 |
| `02-1_IOCP_MultiClient_Server/` | 다중 클라이언트 지원 서버 (IOCP 기반, 고성능 비동기 처리) |
| `03_Packet_Structure/` | 패킷(Header+Body) 구조 통신 설계 및 구현 |
| `04_Session_Room_System/` | 접속 세션 및 방(Room) 관리 시스템 구현 |
| `05_Mini_MMO_World/` | 로그인, 이동, 채팅이 가능한 간단한 MMO 서버 구현 |

---

## 🛠️ 개발 환경
- Language: **C++17**
- IDE: **Visual Studio 2022**, **Cursor**
- Platform: **Windows 10**
- Network API: **Winsock2**

---

## 🚀 현재 진행 단계
> **1단계 – Simple TCP Server/Client 연결 구현 (완료 ✅)**

- 서버: 소켓 생성 → bind → listen → accept → recv/send
- 클라이언트: 소켓 생성 → connect → send → recv
- 기본적인 문자열 통신 성공

> **2단계 – Multi Client Server (스레드 기반) (완료 ✅)**

- 서버: 소켓 생성 → bind → listen → accept (멀티 클라이언트 대응)
- 각 클라이언트 접속 시 별도 스레드 생성 후 통신 처리
- 서버 종료 시 Ctrl+C 입력을 감지하여 소켓 및 스레드 정리 구현
- (추후 IOCP 기반 서버로 리팩터링 예정)

> **2-0단계 – Non-Blocking Socket Server (완료 ✅)**

- 서버: 소켓을 Non-Blocking 모드로 설정
- select()를 사용하여 읽기 가능한 소켓만 처리
- 블로킹 없이 다수 클라이언트를 효율적으로 처리
- (추후 IOCP 기반 서버로 고성능 비동기 모델 전환 예정)

> **2-1단계 – IOCP 기반 Multi Client Server (진행중 🚧)**

- 서버: IOCP 기반 비동기 소켓 통신 구현
- Completion Port를 사용하여 수천 단위 클라이언트를 소수 스레드로 처리
- Worker 스레드 관리 및 Overlapped I/O 구조 설계

---

## ✨ 앞으로의 계획
- 멀티 클라이언트 동시 처리
- 패킷 구조 설계 및 적용
- 세션/방 관리 시스템 구축
- 간단한 MMO 환경 구성

---

## 📢 주의사항
- 모든 프로젝트는 학습/연습을 목적으로 작성되었습니다.
- 실제 상용 서버와는 차이가 있을 수 있습니다.

---

## 🙌 Special Thanks
- 제 실력을 키우기 위해 직접 설계하고 작성한 학습 기록입니다.

