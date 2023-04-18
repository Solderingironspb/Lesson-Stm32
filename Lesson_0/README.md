# STM32. C чего начать? Быстрый старт после Arduino.
##
![0](https://user-images.githubusercontent.com/68805120/91702089-e5aec980-eb80-11ea-95e1-00d8e425a7da.jpg)
- ***Для начала просмотрите вступительное видео.*** Урок#00: [STM32. C чего начать? Быстрый старт после Arduino](https://youtu.be/kYrOqSpjNe0) 
##
**Как подключить китайский программатор:**
![arm-video5-13](https://user-images.githubusercontent.com/68805120/91702590-a03ecc00-eb81-11ea-84ea-d197e2b03772.jpg)
Китайский программатор подключается очень просто.
![8334de3f3836cacc6020d8824d4e5fc7](https://user-images.githubusercontent.com/68805120/91702800-ec8a0c00-eb81-11ea-8f2b-3ccd73eef37d.png)
Для загрузки прошивки в микроконтроллеры STM32 используется программатор ST-LINK/V2, который можно подключить к программируемому микроконтроллеру по интерфейсу SWD. Кроме того, с помощью того же ST-LINK/V2 и SWD можно выполнять пошаговую отладку прошивки из среды разработки с поддержкой точек останова (breakpoint). К этому мы подойдем чуточку позже.
###
Подключение ST-LINK/V2 к плате Blue-Pill осуществляется по 4 проводам. Gnd к Gnd, 3.3v от программатора идут на 3.3v отладочной платы(т.е. плата запитывается от программатора). Контакты DCLK и DIO, расположенные на отладочной плате, подключаются соответственно к контактам SWCLK и SWDIO на программаторе. Также у SWD есть вывод SWO, его подключать необязательно, но если его подключить, то можно выводить сообщения в режиме реального времени. То есть в процессе исполнения кода, МК может слать нам отладочную информацию, например, какой участок кода сейчас выполняется, получается что-то типа uart c терминалом(как Serial.print в Arduino). На китайском программаторе вывод SWO отсутствует, но Вы можете перепаять его и пользоваться. 
### 
Как это сделать? **[[(см. Урок8: STM32. SWO. Доделываем китайский ST_LINK/V2. Используем SWV для отладки.)]](https://youtu.be/NYWTS3dhKJM)**
##
**Если же у Вас оригинальный программатор**
![DOC004099384](https://user-images.githubusercontent.com/68805120/91706719-712b5900-eb87-11ea-8516-9ef14048f003.jpg)
![Снимок](https://user-images.githubusercontent.com/68805120/91707397-6ae9ac80-eb88-11ea-95b0-95deb8504857.PNG)
##
**Если же у Вас ST-link v2.1 от Nucleo**
![image](https://user-images.githubusercontent.com/68805120/232831032-ed5c84ad-4af4-49d3-a8cf-eab69e88ac28.png)
![image](https://user-images.githubusercontent.com/68805120/232831275-7388f4d8-2b16-4661-818e-b22b6cbaf96b.png)


Я очень долго тупил, как оно должно работать, пока не понял, что 1 контакт - это не питание с программатора на отладочную плату, а наоборот питание с отладочной платы на программатор. Отладочную плату можете запитать отдельно. Например через USB. (Это Вам даст ровные 3.3v на плате. От чего с АЦП будет работать одно удовольствие.)
**[[Документация на оригинальный ST-LINK/V2]](https://www.st.com/resource/en/user_manual/dm00026748-stlinkv2-incircuit-debuggerprogrammer-for-stm8-and-stm32-stmicroelectronics.pdf)**

## Ссылки:
###
- [Группа Вконтакте](https://vk.com/solderingiron.stm32) 
###
- [CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) 
###
- [STM32 ST-LINK utility](https://www.st.com/en/development-tools/stsw-link004.html)
###
- [CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) 
###
- [Atollic TrueStudio](https://atollic.com/resources/download/) 
###
- [64-bit Java for Windows](https://www.java.com/en/download/windows-64bit.jsp)



**[[Главная страница]](https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md)**