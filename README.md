
# *Mechanics implemented in the project*

## Character
- The [FPSTestCharacter] class is used for the character (https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.h)
- Various settings have been implemented for the character
  
![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/80fca040-36dc-4c46-a9c5-57c93eed304b)

You can customize: animations, sounds, effects, speed of transition to aiming mode and back, initial weapon, health points.

- The character can take damage from enemies and die if health points are below 0
- Implemented animation blueprint, taking into account different poses of animations and transitions between them. Content/_project/Character/ABP_FPSTestCharacter.uasset

  ![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/6a8361d9-e9d7-4920-9e2a-d2f1d92935cf)
- Can pick up weapons on the level (E button)
- Implemented widgets for weapons and health points on the screen

## Weapon
Made base class for pickups [Item](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.h) Made weapon class based on it [Weapon]( https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.h) which can be selected per level.

- A separate widget has been implemented for the weapon being picked up, which shows: the name, the number of cartridges, the level of the weapon (the number of stars), the button for selection.

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/9d7caf8e-ff08-417c-a7e2-3869c1da4b13)

- For weapons, a separate widget is implemented that displays: the name of the weapon, the number of cartridges and the type of cartridges

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/4ef62dbd-2e45-469d-983f-fa0afa10b819)

- The instance of each weapon can be configured directly in the radar by changing its characteristics

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/e58e1698-ac9f-4bbe-bebb-1571f0769da7)

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/2bd180f9-74e7-4fb4-99bc-45c05b1c67e9)

- The difference in damage to the head and body is taken into account. You can change the damage indicators in the weapon settings
- Implemented single shooting and burst shooting (hold LMB)
- Implemented aiming mode (hold RMB)

## Reload

- Each weapon instance has its own ammo counter. The number of rounds can be set for any weapon.
- A system has been set up that allows you to visualize reloading. When reloading, the clip moves with the character's hand.

## Enemies

- The [Enemy] class is used for the enemy (https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.h)
- There are many settings for the enemy instance right in the editor

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/f506c77e-28d9-4918-8c07-a13a1ca27bb0)

You can customize: animations, sounds, effects, number of hit points, stun chance when hitting an enemy, damage inflicted on the character, delay between attacks, delay before destroying the actor after death

A simple behavior tree has been implemented for the Enemy class.

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/326b4a13-5f4e-4a7f-b3db-ebabc9f28cea)

You can set up patrol points. There is an agro zone upon entering which the enemy will begin to pursue and attack the character, as well as the attack is provoked by shooting at the enemy.
- The enemy can take damage and deal damage.

# *Project file structure*

## C++ files

Inside the C++ code, I tried to add comments to the code as much as possible to describe the mechanics. Most comments are within the character class.

Character:

[FPSTestCharacter.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.h)
[FPSTestCharacter.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.cpp)

Character animation:

[FPSTestCharacterAnimation.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacterAnimation.h)
[FPSTestCharacterAnimation.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacterAnimation.cpp)

Enemy:

[Enemy.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.h)
[Enemy.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.cpp)

Weapon: 

[Weapon.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.h)
[Weapon.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.cpp)

[Item.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.h)
[Item.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.cpp)

## Unreal Engine assets

All custom project files are located in the Content / _project folder

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/ca98d731-c3b9-4c32-b79a-68e70c48c5f0)

AI - Singing Tree for Enemy and AI Controller

Animations - Everything related to animation. Montages, animations, blend space

Character - Core files for the character

Core - Core project files. GM and Controller

Enemy - Core files for the enemy

FX - Special effects for shots of hits on objects and characters

HUD - All widgets for the project. HUD widget and additional widgets for weapons, ammo, health, etc.

Sound - Sounds for the project. Reloading, shooting, melee weapons, etc.

Weapons - Core files for weapons

# Video demonstration

https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/c1249891-2ffb-44c8-b771-075b3235f8c1

# *Реализованные в проекте механики* 

## Персонаж 
- Для персонажа используется класс [FPSTestCharacter](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.h)
- Для персонажа реализованы разные настройки
  
![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/80fca040-36dc-4c46-a9c5-57c93eed304b)

Можно настраивать: анимации, звуки, эффекты, скорость перехода в режим прицеливания и обратно, начальное оружие, очки здоровья.

- Персонаж может получать урон от врагов и умирать если очки здоровья ниже 0
- Реализован анимациионый блюпринт с учетом разных поз анимаций и переходов между ними. Content/_project/Character/ABP_FPSTestCharacter.uasset

  ![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/6a8361d9-e9d7-4920-9e2a-d2f1d92935cf)
- Может побирать оружие на уровне (Кнопка E)
- Реализованы виджеты оружия и очков здоровья на экране 

## Оружие
Сделан базовый класс для подбираемых предметов [Item](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.h) На его основе сделан класс оружия [Weapon](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.h) которое можно подбирать на уровне. 

- Для подбираемого оружия реализован отдельный виджет который показывает: название, количество патронов, уровень оружия (количество звезд), кнопка для подбора. 

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/9d7caf8e-ff08-417c-a7e2-3869c1da4b13)

- Для оружия реализован отдельный виджет выводящий на экран: название оружия, количество патронов и тип патронов

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/4ef62dbd-2e45-469d-983f-fa0afa10b819)

- Инстанс каждого оружия можно настраивать прямо в радакторе изменяя его характеристики

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/e58e1698-ac9f-4bbe-bebb-1571f0769da7)

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/2bd180f9-74e7-4fb4-99bc-45c05b1c67e9)

- Учитывается разница урона в голову и тела. Изменить показатели урона можно в настройках оружия
- Реализована одиночная стрельба и стрельба очередью (зажать ЛКМ)
- Реализован режим прицеливания (зажать ПКМ) 

## Перезарядка

- У каждого инстанса оружия есть свой счетчик патронов. Количество патронов можно задавать у любого оружия. 
- Настроена система позволяющая вузуализировать перезарядку. При перезарядке обойма перемещается вместе с рукой персонажа.
 
## Враги 

- Для врага используется класс [Enemy](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.h)
- Для инстанса врага есть множество настроек проямо в редакторе

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/f506c77e-28d9-4918-8c07-a13a1ca27bb0)

Можно настраивать: анимации, звуки, эффекты, количество очков жизни, шанс стана при попадание по врагу, наносимый персонажу урон, задержку емжду атаками, задержку перед уничтожением актора после смерти

Для класса Enemy реализованна простое дерево поведения. 

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/326b4a13-5f4e-4a7f-b3db-ebabc9f28cea)

Можно настраивать точки для патрулирования. Есть агро зона войдя в которую враг начнет преследовать и атаковать персонажа, так же атаку провоцирует стрельба про врагу. 
- Враг может получать урон и наносить урон.

# *Структура файлов проекта*

## C++ файлы 

Внутри С++ кода постарался максимально добавить комментарии к коду для описания механик. Больще всего комментариев внутри класса персонажа.

Персонаж:

[FPSTestCharacter.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.h)
[FPSTestCharacter.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacter.cpp)

Анимация персонажа:

[FPSTestCharacterAnimation.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacterAnimation.h)
[FPSTestCharacterAnimation.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/FPSTestCharacterAnimation.cpp)

Враг:

[Enemy.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.h)
[Enemy.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Enemy.cpp)

Оружие и его базовый класс предмет: 

[Weapon.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.h)
[Weapon.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Weapon.cpp)

[Item.h](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.h)
[Item.cpp](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/blob/main/Source/FPSTest/Item.cpp)

## Unreal Engine assets 

Все кастомные файлы проекта находятся в папке Content/_project

![image](https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/ca98d731-c3b9-4c32-b79a-68e70c48c5f0)

AI - Дерево певедения для врага и AI Controller

Animations - Все что касается анимации. Монтажи, анимации, blend space 

Character - Core файлы для персонажа 

Core - Core файлы проекта. GM и Controller 

Enemy -  Core файлы для врага

FX - Спецэффекты для выстрелов попаданий по объектам и персонажам

HUD - Все виджеты для проекта. HUD виджет и дополнительные виджеты для оружия, патронов, здоровья и тд. 

Sound - Звуки для проекта. Перезарядка, стрельба, холодное оружие и тд. 

Weapons - Core файлы для оружия

# Видео демонстрация 

https://github.com/Kirill-Geskin/TestFPS-UE4.27.2/assets/80645926/c1249891-2ffb-44c8-b771-075b3235f8c1
