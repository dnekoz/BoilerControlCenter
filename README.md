# BoilerControlCenter
Требования:
1. Контроль давления в системе отопления, отключение в случае падения давления ниже 1,0 bar или выше 2,8 bar;
2. Управление через OpenTherm температурой теплоносителя; 
3. Часы точного времени;
4. Управление температурой в комнате через разрыв термостата;
5. Перезапуск котла удалённо;
6. Протокол взаимодействия - MQTT;
7. Датчик утечки природного газа MQ4;
8. Датчик концентрации CO2 (надо выбрать датчик: senseAir S8 004-0-0053 или Winsen MH-Z19C)

Тех решения:
1. Замена штатного датчика давления в котле на XIDIBEI (0-5МПа) + НО-реле;
2. OpenTherm-адаптер, подключаемый через реле-переключатель (? если не взлетит);
3. Часы на базе модуля ds3231;
4. НО-Реле + датчик температуры DS18B20 + внешний датчик, подключенный по радиоканалу;
5. НО-Реле;
6. Решается при помощи библиотеки.
