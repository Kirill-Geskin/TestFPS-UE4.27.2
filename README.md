
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

Все кастомные файлы проекта находяться в папке Content/_project

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
