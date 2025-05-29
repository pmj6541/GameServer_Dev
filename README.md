# 🎮 GameServer_Dev_Study

C++ 기반 게임 서버 개발자로 성장하기 위한 실전 프로젝트 학습 저장소입니다.  
TCP 통신부터 IOCP 기반 고성능 서버, 패킷 구조화, 세션/룸 시스템, 간단한 MMO 서버 구현까지 전 과정을 기록합니다.

---

## 📚 학습 목표

- C++ 네트워크 프로그래밍 기초 (TCP, UDP)
- 멀티 클라이언트 처리 구조 및 IOCP 모델 이해
- 스트림 기반 안전한 패킷 프로토콜 설계
- 세션 및 방(Room) 관리 시스템 구현
- 간단한 MMO 서버 제작

---

## 📂 프로젝트 구조

| 단계 | 설명 |
|:--|:--|
| `01_Simple_TCP_Connection/` | TCP 기반 단일 클라이언트 통신 구현 |
| └ `SimpleTCPServer/`, `SimpleTCPClient/` | 서버/클라이언트 기본 구조 실습 |
| `02_MultiClient_Server/` | 다중 클라이언트 처리 구조 구현 |
| └ `NonBlockingSocketServer/` | select 기반 논블로킹 서버 |
| └ `IOCPMultiClientServer/` | IOCP 기반 비동기 고성능 서버 |
| └ `TestClient/` | 대량 클라이언트 부하 테스트 도구 |
| `03_Packet_Structure/` | 패킷 프로토콜 정의 및 직렬화/역직렬화 처리 |
| `04_Session_Room_System/` | 접속 세션 및 Room 매칭 구조 설계 |
| `05_Mini_MMO_World/` | 강화, 이동, 채팅이 가능한 미니 MMO 구현 예정 |

---

## 🛠️ 개발 환경

- **Language**: C++17
- **IDE**: Visual Studio 2022, Cursor
- **OS**: Windows 10
- **API**: Winsock2 (IOCP)

---

## 🚀 현재 진행 단계

### ✅ 1단계 – 단일 TCP 서버/클라이언트 구현
- 기본 소켓 통신 구조 확인 (send/recv)

### ✅ 2단계 – 다중 클라이언트 처리
- 스레드 기반 → select() → IOCP 비동기 구조 순으로 발전

### ✅ 2-1단계 – IOCP 기반 비동기 처리
- CompletionPort, Overlapped I/O 구조 도입
- Overlapped 수신 → OpCode → Handler Dispatch 처리 구조 확립

### ✅ 2-2단계 – 부하 테스트 클라이언트
- 1000개 이상의 클라이언트 연결, 처리 성능 확인

### 🚧 3단계 – 패킷 구조 설계 및 구현 (진행중)
- TCP 스트림 대응을 위한 길이 기반 패킷 구조 설계
- ByteBuffer 유틸리티 기반 직렬화 / 역직렬화 처리
- 메이플스토리 강화 시스템 구조를 반영한 `EnhanceRequest / EnhanceResponse` 구현
- HandlerManager를 통한 OpCode → Handler 구조 정립

---

## 🎯 향후 계획

- [ ] 패킷 구조에 기반한 Room/Session 분리 처리
- [ ] 유저 입장 / 퇴장 처리 로직 구축
- [ ] 간단한 강화/채팅 시스템을 포함한 MMO 기본 골격 구현

---

## ⚠️ 주의사항

- 모든 구현은 학습용이며, 실제 상용 서버 구조와는 다를 수 있습니다.

---

## 🙌 Special Thanks

- 본 프로젝트는 라이브 서버 프로그래머를 목표로 삼아 설계된 개인 학습 기록입니다.
