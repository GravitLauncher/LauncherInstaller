# LauncherInstaller
Установщик https://github.com/GravitLauncher/Launcher на Linux машины  
# Использование
- Скачайте последний релиз установщика, залейте его на хостинг  
- Запустите его  
  
Если вы запускаете от Root то увидите такой вывод:
```
[WARN] Installing on behalf of the superuser is prohibited
[ROOT] Check package installation
Create new user for LaunchServer [YES/NO]? yes
Username[launchserver]: launchserver
New UID [2001]: 2001
[ROOT] Create user launchserver
[ROOT] Change dir to /home/launchserver
```
Далее вам нужно будет выбрать версию, например dev:  
```
Avalible versions: [dev, 5.0.0b6, 5.0.0b7]
Print version:dev
UID: 2001
Download LaunchServer
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 5102k  100 5102k    0     0   902k      0  0:00:05  0:00:05 --:--:-- 1042k
```
Начнется скачка jar файла и библиотек. Дождитесь её завершения  
Введите адрес, на котором будет работать лаунчсервер и имя вашего проекта  
```
LaunchServer address(default: localhost): 
localhost
LaunchServer projectName: 
SuperDuperCraft
```
Дождитесь окончания проверки  
Ответьте на вопрос `нужен ли вам screen`
```
Create start script. Using screen [YES/NO]? yes
```
Ответьте на вопрос `скачать дополнительные модули`  
Если вы не уверены в назначении какого либо модуля - НЕ СТАВЬТЕ ЕГО  
В качестве примера мы установим модуль AutoSaveSessions  
```
Download modules[YES/NO]?yes
Print module name:AutoSaveSessions
Download AutoSaveSessions.jar
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  4223  100  4223    0     0   7541      0 --:--:-- --:--:-- --:--:--  7527
```
Как только вы ввели все необходимые модули напишите stop что бы остановить выбор  
```
Print module name:stop
```
Поздравляем! Вы установили LaunchServer  
Запустить его можно с помощью скрипта start.sh  
Актуальная Wiki лаунчера: https://launcher.gravit.pro  
#Опции запуска установщика
`./installer -m` Не выполнять установку лаунчсервера. Это необходимо если вы собираетесь добавить модули в уже существующий лаунчсервер
`./installer -b` Собрать лаунчсервер из исходников
