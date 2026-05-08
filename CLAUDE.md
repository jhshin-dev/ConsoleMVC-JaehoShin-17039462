# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**S-Semi 반도체 시료 생산주문관리 시스템** — 콘솔 기반 C++20 애플리케이션.  
주문 담당자·생산 담당자가 시료 등록, 주문 처리, 재고 관리, 공정 현황을 콘솔 명령으로 운영한다.

> **현재 단계: PoC (Proof of Concept)**  
> 핵심 로직(주문 상태 머신, 재고 처리, JSON 영속성)의 작동 여부를 확인하는 것이 목적이다.  
> 예외 처리 완성도·UI 완성도보다 핵심 흐름이 올바르게 동작하는지를 우선한다.
>
> **PoC 완료 목표**: `model/` · `controller/` · `view/` 패키지 구조를 실제로 분리 구현하고,  
> 각 레이어의 역할(Model = 데이터·영속성, Controller = 비즈니스 로직·입력 처리, View = 콘솔 출력 전담)이  
> 코드 수준에서 명확히 경계가 그어진 상태로 빌드·실행 가능한 것.

## Build

**IDE**: Visual Studio 2022 (v145 toolset), solution file `ConsoleMVC.slnx`  
**Standard**: C++20 (`stdcpp20`), SDL check 활성화, Warning Level 3  
**Target**: Windows x64 Console Application

```
# IDE에서
Ctrl+Shift+B   — 솔루션 빌드
F5             — 디버그 빌드 & 실행
Ctrl+F5        — 릴리즈 빌드 & 실행

# MSBuild CLI (Developer Command Prompt)
msbuild ConsoleMVC.vcxproj /p:Configuration=Debug /p:Platform=x64
msbuild ConsoleMVC.vcxproj /p:Configuration=Release /p:Platform=x64
```

소스 파일을 추가할 때는 `ConsoleMVC.vcxproj`의 `<ItemGroup>`에 `<ClCompile>` / `<ClInclude>` 항목을 등록해야 빌드에 포함된다.

## Architecture: MVC (Console)

```
ConsoleMVC/
├── model/          도메인 엔티티 & 상태 머신 (순수 로직, I/O 없음)
│   ├── Sample      시료 정보 (ID, 이름, 재고 수량)
│   ├── Order       주문 엔티티 + OrderStatus 열거형
│   └── ProductionLine  생산 라인 (시료 1개씩 순차 생산)
├── view/           콘솔 출력 전담 (cin/cout은 여기서만)
│   ├── OrderView
│   └── ProductionView
├── controller/     사용자 입력을 받아 Model 조작 후 View 호출
│   ├── OrderController
│   └── ProductionController
└── main.cpp        진입점, 역할(Role) 선택 메뉴 루프
```

- Model은 View·Controller를 include하지 않는다.
- Controller가 Model의 상태를 변경하고 View에 결과를 전달한다.
- 파일명은 클래스명과 1:1로 대응한다 (`Order.h` / `Order.cpp`).

## Domain Rules

### 역할(Role)
| 역할 | 권한 |
|------|------|
| 주문 담당자 | 주문서 작성(RESERVED), 출고 처리(CONFIRMED → RELEASED) |
| 생산 담당자 | 시료 등록, 주문 승인/거절, 생산 완료 처리 |

### 주문 상태 머신 (OrderStatus)
```
RESERVED
  └─[거절]→ REJECTED          (모니터링 제외)
  └─[승인 + 재고 충분]→ CONFIRMED
  └─[승인 + 재고 부족]→ PRODUCING
       └─[생산 완료]→ CONFIRMED
CONFIRMED
  └─[출고 처리]→ RELEASED
```

- REJECTED 주문은 목록 조회·모니터링 대상에서 제외한다.
- 생산 라인은 PRODUCING 주문에 한해서만 생산을 수행한다.
- 재고가 충분한 시료에 대해 추가 생산을 시작해서는 안 된다.

### 생산 라인
- 하나의 생산 라인은 시료를 **한 번에 1개씩** 생산한다.
- 생산 라인은 PRODUCING 상태인 주문 큐를 순서대로 처리한다.
- 생산 완료 시 해당 주문을 CONFIRMED으로 전환하고 재고를 증가시킨다.

## Coding Conventions

- 헤더 가드 대신 `#pragma once` 사용
- `std::string` (UTF-8), `std::vector`, `std::map`/`std::unordered_map` 우선 사용
- ID는 정수(int 또는 size_t)로 관리하고, 순차 증가 방식으로 자동 부여
- `enum class OrderStatus` — 암묵적 정수 변환을 막기 위해 scoped enum 사용
- 소멸자가 있는 클래스는 Rule of Five 또는 `= default`/`= delete` 명시
