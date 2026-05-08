# S-Semi 반도체 시료 생산주문관리 시스템

콘솔 기반 C++20 MVC 애플리케이션. 주문 담당자와 생산 담당자가 시료 등록, 주문 처리, 재고 관리, 공정 현황을 콘솔 명령으로 운영한다.

> **현재 단계: PoC 완료** — MVC 레이어 분리 구현 및 핵심 흐름(주문 상태 머신, 재고 처리, JSON 영속성) 동작 검증 완료.

---

## 빌드 및 실행

**요구 환경**: Visual Studio 2022, MSVC v145, Windows x64

```
# Visual Studio IDE
Ctrl+Shift+B   — 빌드
F5             — 디버그 실행

# MSBuild CLI (Developer Command Prompt)
msbuild ConsoleMVC.vcxproj /p:Configuration=Debug /p:Platform=x64
```

빌드 후 `x64\Debug\ConsoleMVC.exe` 를 실행한다. 데이터 파일은 실행 파일과 같은 디렉토리의 `data\` 폴더에 자동 생성된다.

---

## 주문 상태 흐름

```
RESERVED
  └─[거절]──────────────→ REJECTED        (목록·모니터링 제외)
  └─[승인 + 재고 충분]──→ CONFIRMED
  └─[승인 + 재고 부족]──→ PRODUCING
                               └─[생산 완료]→ CONFIRMED
                                                  └─[출고]→ RELEASED
```

---

## 역할별 기능

### 주문 담당자
| 메뉴 | 동작 |
|------|------|
| 주문 등록 | 시료 ID·수량 입력 → RESERVED 주문 생성 |
| 주문 목록 조회 | REJECTED 제외 전체 주문 표시 |
| 출고 처리 | CONFIRMED 주문 선택 → RELEASED 전환 |

### 생산 담당자
| 메뉴 | 동작 |
|------|------|
| 시료 등록 | 이름·초기 재고 입력 → 시료 생성 |
| 시료 목록 조회 | 전체 시료 및 현재 재고 표시 |
| 주문 승인 | 재고 충분 → CONFIRMED / 부족 → PRODUCING |
| 주문 거절 | RESERVED → REJECTED |
| 생산 현황 조회 | PRODUCING 주문 목록 표시 |
| 생산 완료 처리 | PRODUCING → CONFIRMED |

---

## 아키텍처

```
ConsoleMVC/
├── model/          데이터 구조체 + JSON 영속성 (I/O 없음)
│   ├── OrderStatus.h         enum class + 직렬화 유틸
│   ├── Sample.h / Order.h    도메인 구조체
│   ├── SampleRepository      data/samples.json 읽기·쓰기
│   └── OrderRepository       data/orders.json 읽기·쓰기
├── view/           콘솔 I/O 전담 (cin/cout은 여기서만)
│   ├── OrderView             주문 담당자 화면·입력
│   └── ProductionView        생산 담당자 화면·입력
├── controller/     비즈니스 로직 (cin/cout 없음)
│   ├── OrderController
│   └── ProductionController
├── lib/json.hpp    nlohmann/json v3.11.3 (단일 헤더)
└── main.cpp        역할 선택 루프
```

**레이어 규칙**
- Model → View·Controller 방향의 include 금지
- Controller는 View 메서드를 통해서만 입출력
- 상태 변경 직후 Repository `save()` 즉시 호출

---

## 데이터 파일

프로그램 재시작 후에도 데이터가 유지된다.

```
data/
├── samples.json    시료 목록 + nextId
└── orders.json     주문 목록 + nextId
```

```json
// samples.json 예시
{ "nextId": 2, "samples": [{ "id": 1, "name": "GaN-100", "stock": 7 }] }

// orders.json 예시
{ "nextId": 3, "orders": [{ "id": 1, "sampleId": 1, "quantity": 3, "status": "RELEASED" }] }
```
