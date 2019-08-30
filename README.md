# 3DMultiAnimationInstance
Projekt na studia 2012/2013 r. Projekt zwielokrotniania animowanych obiektów 3d

## Cel zadania
Celem zadania było zaimplementowanie aplikacji wykorzystującej sprzętowe zwielokrotnianie animowanych modeli (Multi Animation Instance). W praktyce zostanie zaimplementowana animacja chodzenia z wykorzystaniem załadowanego jednokrotnie modelu, do pamięci karty graficznej. Następnie nastąpi multiplikacja kontrolera animacji dla każdego z kolejnych modeli (ostatecznie kilkaset).

## Zasada działania
Podczas inicjalizacji aplikacji tworzony jest obiekt Kontenera animacji (AnimationContainer), który zajmuję się procesami:

*	ładowania struktury modelu (wierzchołki, współrzędne teksturowania, dane dot. tekstur) z hierarchią kości do pamięci karty graficznej (GPU)
*	załadowaniem tekstur do pamięci GPU
*	kompilacją shaderów

Następnie dla każdego z modeli (Model) są tworzone instancje Kontrolera Animacji (AnimationInstance), które zajmują się kontrolą animacji danego modelu.

Każdy z modeli oprócz kontrolera animacji posiada macierz widoku projekcji oraz macierz świata, pozwalającą na rendering w określonej pozycji na scenie.

Podczas renderowania każdego z modeli, przed samym procesem rysowania jest modyfikowana pozycja kości na aktualna dla aktualnego modelu, następnie jest uruchamiany potok renderingu.

