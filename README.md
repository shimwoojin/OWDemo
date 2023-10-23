# OWDemo
UE 4.26 C++, RPG (Weapons, Inventory, SaveGame)

동영상 링크 : https://www.youtube.com/watch?v=N8sijH3GqfE

!언리얼 엔진 4.26 버전으로 만들었으며 엔진 4.26 버전이 다운로드 되어 있다면 프로젝트를 다운받아 플레이해 보실 수 있습니다!

!조작법!

- [이동 : w,a,s,d]
- [모드 전환 : Tab]
- [점프 : Space]
- [앉기 : Ctrl]
- [무기 변경 : 1,2,3,4]
- [장비창 : u]
- [인벤토리창 : i]
- [닷지 : Shift]
- [공격 : 마우스 좌클릭(길게 누르기 포함)]
- [공격 보조 : 마우스 우클릭(길게 누르기 포함)]

!코드!

- Source 폴더 내 C++ 코드 있음
- OWDemo : 메인 로직을 담당하는 모듈
- OWDemoSetting : OWDemo의 세팅 값을 다루는 모듈

!상세!

- 캐릭터와 플레이어의 인터페이스를 세분화 하여 더 나은 객체지향적인 설계를 하고자 함
- Rifle의 Bullet, Sniper의 Bullet, Pistol의 Bullet 등 클래스가 많아지는 것을 염려하여 Bullet에 디자인 패턴을 적용 (생성 - 빌더)
- 무기 스왑 시 UI가 바뀐다거나 캐릭터의 무기 별 자세가 바뀌어야 하는 부분은 델리게이트를 사용하여 유연한 구조, 일종의 옵저버 패턴
- 충돌과 관련하여 각 개체를 구분하여 Obejct Type과 Preset을 작성 -> Preset에 대한 문자열 의존성을 줄이기 위하여 서브 모듈의 const static 키워드를 사용하여 관리
