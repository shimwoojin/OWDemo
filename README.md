# ⚔️ OWDemo - Action RPG

> **Unreal Engine 4.26 C++**로 구현한 3D 액션 RPG 게임

[![Video](https://img.youtube.com/vi/N8sijH3GqfE/maxresdefault.jpg)](https://www.youtube.com/watch?v=N8sijH3GqfE)
**▶️ [YouTube 플레이 영상 보기](https://www.youtube.com/watch?v=N8sijH3GqfE)**

---

## 📋 프로젝트 개요

- **개발 기간**: 2023.07 ~ 2023.08 (2개월)
- **개발 인원**: 1인 (개인 프로젝트)
- **장르**: 3D Action RPG
- **목적**: C++ 기반 게임 시스템 설계 및 디자인 패턴 학습

---

## 🛠 기술 스택

| 분류 | 기술 |
|------|------|
| **엔진** | Unreal Engine 4.26 |
| **언어** | C++ |
| **아키텍처** | Component-Based Architecture |
| **디자인 패턴** | Factory Pattern, Builder Pattern, Delegate Pattern |
| **모듈 구조** | OWDemo (메인), OWDemoSetting (설정) |

---

## ✨ 주요 구현 기능

### 1️⃣ 다양한 무기 시스템 (8종)
무기는 상속 구조로 설계되어 확장 가능:

**🔫 원거리 무기 (Ranged Weapon)**
- **Assault Rifle** (돌격소총)
- **Sniper Rifle** (저격총)
- **Pistol** (권총)
- **Bow** (활)
- **Staff** (마법 지팡이)

**⚔️ 근접 무기 (Melee Weapon)**
- **Sword** (검)
- **Shield** (방패)

**🎯 무기 시스템 특징**
- 무기별 고유 공격 패턴 (좌클릭, 우클릭, 홀드)
- 무기 스왑 시스템 (1, 2, 3, 4 키)
- 무기별 자세 및 애니메이션 전환
- Delegate 기반 UI 연동

### 2️⃣ 투사체 시스템 (Builder Pattern 적용)
```cpp
// 문제: Rifle의 Bullet, Sniper의 Bullet, Pistol의 Bullet 등 클래스 증가
// 해결: Factory Pattern으로 Bullet 생성 통합
```

**투사체 종류**:
- 🔹 **Bullet**: 총기류 탄환 (Factory로 생성)
- 🏹 **Arrow**: 화살
- ⚡ **ThunderBall**: 번개 마법
- 💧 **WaterBall**: 물 마법
- 🔥 **ElementalBall**: 원소 마법

### 3️⃣ 인벤토리 & 장비 시스템
- **InventoryComponent**: 아이템 관리
- **Equipment System**: 무기 장착/해제
- **Item Pickup**: 월드 아이템 획득
- **TreasureBox**: 보물 상자 시스템

**UI 기능**:
- 인벤토리 창 (I 키)
- 장비 창 (U 키)
- 드래그 앤 드롭
- 아이템 정보 툴팁

### 4️⃣ SaveGame 시스템
```cpp
// UniqueID = ItemName + GUID
// 겹칠 수 없는 슬롯 네임으로 데이터 저장/로드
```

- 캐릭터 스탯 저장
- 인벤토리 상태 저장
- 장비 상태 저장
- 게임 진행도 저장
- UniqueID 기반 데이터 관리

### 5️⃣ 컴포넌트 기반 캐릭터 시스템
각 기능을 독립적인 컴포넌트로 분리:

| 컴포넌트 | 역할 |
|---------|------|
| **StatComponent** | HP, MP, 스탯 관리 |
| **StateComponent** | 캐릭터 상태 관리 (Idle, Attack, Dodge 등) |
| **WeaponComponent** | 무기 장착 및 관리 |
| **InventoryComponent** | 아이템 인벤토리 |
| **ModeManageComponent** | 전투/탐색 모드 전환 |
| **AttackTypeComponent** | 공격 타입 처리 |

**장점**:
- ✅ 재사용성 향상
- ✅ 기능 단위 독립성
- ✅ 유지보수 용이

### 6️⃣ 전투 시스템
- **모드 전환**: 전투/탐색 모드 (Tab)
- **Dodge**: 회피 액션 (Shift)
- **공격 타입**: 기본 공격 / 차징 공격 / 보조 공격
- **Camera Shake**: 타격감 연출
- **Hit Reaction**: 피격 반응

### 7️⃣ AI 시스템
- **BaseEnemy**: 적 캐릭터 베이스 클래스
- **Griffon**: 보스 몬스터
- **AI Controller**: 적 AI 제어
- **Behavior Tree**: 행동 패턴 (추정)

---

## 🎮 조작법

| 키 | 기능 |
|---|------|
| **W, A, S, D** | 이동 |
| **Tab** | 전투/탐색 모드 전환 |
| **Space** | 점프 |
| **Ctrl** | 앉기 |
| **Shift** | 회피 (Dodge) |
| **1, 2, 3, 4** | 무기 변경 |
| **U** | 장비창 열기 |
| **I** | 인벤토리창 열기 |
| **마우스 좌클릭** | 공격 (홀드 가능) |
| **마우스 우클릭** | 보조 공격 (홀드 가능) |

---

## 📁 프로젝트 구조

### C++ 소스 코드
```
OWDemo/
├── Source/
│   ├── OWDemo/                    # 메인 게임 로직 모듈
│   │   ├── Public/
│   │   │   ├── GamePlay/
│   │   │   │   ├── Character/     # 캐릭터 시스템
│   │   │   │   │   ├── BaseCharacter.h
│   │   │   │   │   ├── BasePlayer.h
│   │   │   │   │   ├── PlayerShinbi.h
│   │   │   │   │   ├── BaseEnemy.h
│   │   │   │   │   ├── Griffon.h
│   │   │   │   │   ├── Animation/
│   │   │   │   │   ├── Controller/
│   │   │   │   │   └── AI/
│   │   │   │   ├── Component/     # 컴포넌트 시스템
│   │   │   │   │   ├── StatComponent.h
│   │   │   │   │   ├── StateComponent.h
│   │   │   │   │   ├── WeaponComponent.h
│   │   │   │   │   ├── InventoryComponent.h
│   │   │   │   │   ├── ModeManageComponent.h
│   │   │   │   │   └── AttackTypeComponent.h
│   │   │   │   ├── Item/          # 아이템 시스템
│   │   │   │   │   ├── Item.h
│   │   │   │   │   ├── WorldItem.h
│   │   │   │   │   ├── InventoryItem.h
│   │   │   │   │   ├── TreasureBox.h
│   │   │   │   │   └── Weapon/    # 무기
│   │   │   │   │       ├── BaseWeapon.h
│   │   │   │   │       ├── RangedWeapon.h
│   │   │   │   │       │   ├── BaseGun.h
│   │   │   │   │       │   │   ├── AssaultRifle.h
│   │   │   │   │       │   │   ├── Pistol.h
│   │   │   │   │       │   │   └── SniperRifle.h
│   │   │   │   │       │   ├── BaseBow.h
│   │   │   │   │       │   └── BaseStaff.h
│   │   │   │   │       └── MeleeWeapon.h
│   │   │   │   │           ├── BaseSword.h
│   │   │   │   │           └── BaseShield.h
│   │   │   │   ├── Projectile/    # 투사체
│   │   │   │   │   ├── BaseProjectile.h
│   │   │   │   │   ├── DamageableProjectile.h
│   │   │   │   │   ├── Bullet/    # Factory Pattern
│   │   │   │   │   │   ├── Bullet.h
│   │   │   │   │   │   └── BulletFactory.h
│   │   │   │   │   ├── Arrow.h
│   │   │   │   │   ├── ThunderBall.h
│   │   │   │   │   ├── WaterBall.h
│   │   │   │   │   └── ElementalBall.h
│   │   │   │   ├── UI/            # UI 시스템
│   │   │   │   ├── Skill/         # 스킬
│   │   │   │   ├── CameraShake/   # 카메라 효과
│   │   │   │   ├── Notify/        # 애니메이션 노티파이
│   │   │   │   ├── Interface/     # 인터페이스
│   │   │   │   └── Trigger/       # 트리거
│   │   │   └── GameMake/
│   │   └── Private/
│   └── OWDemoSetting/             # 세팅 모듈
│       ├── Public/
│       └── Private/
└── Content/                        # 블루프린트 & 에셋
    ├── Blueprints/
    │   ├── Actors/
    │   ├── UI/
    │   └── BP_OWDemoGameMode
    ├── Assets/
    ├── Datas/
    └── Maps/
```

---

## 🎯 구현 하이라이트

### 🔹 1. 컴포넌트 기반 설계
```cpp
// 캐릭터 기능을 독립적인 컴포넌트로 분리
class ABaseCharacter : public ACharacter
{
    UPROPERTY()
    UStatComponent* StatComponent;
    
    UPROPERTY()
    UWeaponComponent* WeaponComponent;
    
    UPROPERTY()
    UInventoryComponent* InventoryComponent;
    
    UPROPERTY()
    UStateComponent* StateComponent;
};
```

**장점**:
- 기능별 독립성 → 재사용 가능
- 유지보수 용이
- 테스트 단위 명확

### 🔹 2. Factory Pattern으로 Bullet 생성 최적화
```cpp
// Before: 총기마다 Bullet 클래스 생성 필요
// - RifleBullet, PistolBullet, SniperBullet ...

// After: BulletFactory로 통합 생성
class BulletFactory
{
    static Bullet* CreateBullet(EWeaponType Type, FBulletParams Params);
};
```

### 🔹 3. Delegate 기반 UI-Logic 분리
```cpp
// 무기 스왑 시 UI 자동 업데이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, ABaseWeapon*);

class UWeaponComponent : public UActorComponent
{
    FOnWeaponChanged OnWeaponChanged;
    
    void SwapWeapon(int32 Index)
    {
        // 무기 교체 로직
        OnWeaponChanged.Broadcast(CurrentWeapon);
    }
};
```

### 🔹 4. Collision Preset 관리 (OWDemoSetting 모듈)
```cpp
// 문자열 의존성 제거 → const static 관리
class UOWGameSettings : public UObject
{
public:
    static const FName COLLISION_PRESET_PLAYER;
    static const FName COLLISION_PRESET_ENEMY;
    static const FName COLLISION_PRESET_PROJECTILE;
};
```

**장점**:
- 오타 방지
- 리팩토링 용이
- IntelliSense 지원

### 🔹 5. SaveGame 시스템 (UniqueID)
```cpp
// UniqueID = ItemName + GUID
FString UniqueID = ItemName + FGuid::NewGuid().ToString();

// 언제든 원하는 시점에 Load 가능
USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(UniqueID, 0);
```

---

## 🏗️ 모듈 구조

### OWDemo (메인 모듈)
- 게임플레이 로직
- 캐릭터, 아이템, UI, AI 등

### OWDemoSetting (설정 모듈)
- **로딩 단계**: PreDefault (OWDemo보다 먼저 로드)
- **역할**: 게임 세팅 값 관리
  - Collision Preset 상수
  - 게임 전역 설정
  - const static 키워드로 문자열 의존성 제거

---

## 🚀 실행 방법

### 📦 필수 요구사항
- **Unreal Engine 4.26**
- **Visual Studio 2019** 이상 (C++ 개발 도구)
- **Windows 10/11**

### ⚙️ 실행 단계

1. **Unreal Engine 4.26 설치**

2. **프로젝트 열기**
   ```
   OWDemo.uproject 더블클릭
   → Unreal Editor에서 자동 빌드 및 실행
   ```

3. **C++ 코드 수정 시**
   ```
   Visual Studio에서 Source 폴더 열기
   → 코드 수정
   → 빌드 (Ctrl+Shift+B)
   → Unreal Editor에서 Hot Reload
   ```

4. **게임 플레이**
   - 에디터 상단 "플레이" 버튼 또는 `Alt + P`

---

## 📚 학습 성과

### 설계 & 아키텍처
- ✅ **컴포넌트 기반 설계**: 기능 독립성 및 재사용성
- ✅ **디자인 패턴 실무 적용**:
  - Factory Pattern (Bullet 생성)
  - Delegate Pattern (UI-Logic 분리)
- ✅ **모듈 구조 설계**: OWDemo + OWDemoSetting 분리
- ✅ **인터페이스 세분화**: 객체지향 원칙 적용

### 게임 시스템 구현
- ✅ **무기 시스템**: 상속 구조 설계 (8종 무기)
- ✅ **인벤토리 시스템**: 아이템 관리 및 UI 연동
- ✅ **SaveGame 시스템**: UniqueID 기반 데이터 관리
- ✅ **전투 시스템**: 공격 타입, 회피, 카메라 쉐이크

### C++ & Unreal
- ✅ **C++ 클래스 설계**: 상속, 다형성, 캡슐화
- ✅ **Unreal Component 시스템**: ActorComponent 활용
- ✅ **Delegate 시스템**: Multicast Delegate 활용
- ✅ **Collision System**: Object Type, Preset 관리
- ✅ **Animation System**: Notify, Blend Space

---

## 🎨 게임 특징

- 🎭 **플레이어블 캐릭터**: Shinbi
- 👾 **적 캐릭터**: Griffon (보스)
- ⚔️ **8종 무기** (총 5종, 활, 지팡이, 검, 방패)
- 🎒 **인벤토리 & 장비 시스템**
- 💾 **데이터 저장/로드**
- 🎯 **컴포넌트 기반 설계**
- 🏗️ **2개 모듈 구조**

---

## 🔧 기술적 도전 과제

### 1️⃣ 문제: 투사체 클래스 증가
**상황**: 총기마다 Bullet 클래스가 필요 (RifleBullet, PistolBullet...)

**해결**: Factory Pattern 적용
- BulletFactory로 타입별 생성 통합
- 클래스 수 감소 및 생성 로직 중앙화

### 2️⃣ 문제: Collision Preset 문자열 의존
**상황**: 하드코딩된 Preset 이름 → 오타 위험

**해결**: OWDemoSetting 모듈에 const static 관리
- 컴파일 타임 체크
- IntelliSense 지원

### 3️⃣ 문제: UI-Logic 강결합
**상황**: 무기 변경 시 UI 직접 업데이트 → 의존성 문제

**해결**: Delegate Pattern
- WeaponComponent가 OnWeaponChanged 브로드캐스트
- UI는 델리게이트 구독만 → 느슨한 결합

---

## 📌 참고사항

- 이 프로젝트는 학습 목적으로 제작되었습니다
- Unreal Engine 4.26 C++ 기반 RPG 시스템 설계 연습 프로젝트입니다
- 컴포넌트 기반 설계 및 디자인 패턴 실무 적용에 중점을 두었습니다

---

## 📧 Contact

- **Email**: ggoggal@gmail.com
- **Portfolio**: [https://shimwoojin-portfolio.vercel.app/](https://shimwoojin-portfolio.vercel.app/)
- **GitHub**: [@shimwoojin](https://github.com/shimwoojin)

---

**Made with ❤️ by Woojin Shim** | 2023.07 ~ 2023.08
