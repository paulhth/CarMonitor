 # Adresa repository-ului:
 
 https://github.com/paulhth/CarMonitor          - GitHub
 https://gitlab.upt.ro/paul.horvath/CarMonitor  - GitLab
 
 # Rularea proiectului

1. Se identifica portul OBD-II al masinii si se conecteaza cititorul (Vgate iCar Pro V2) la masina.
> Este necesar ca masina sa aiba minim contactul pus dar se recomanda sa se porneasca motorul masinii pentru a accesa parametrii.

2. Se seteaza atat numele dispozitivului OBD-II la care se va conecta ESP-32 cat si SSID si parola retelei din fisierul de configurare, retea in care se va realiza afisarea interfetei.

3. Se conecteaza ESP-32 la o sursa de alimentare. 
> Este recomandat sa se conecteze prin cablu de date USB-A to microUSB la laptopul dotat cu extensia PlatformIO in Visual Studio Code, Arduino IDE, sau orice aplicatie care are integrat un Serial Monitor pentru a urmari statusul lui ESP-32.
> 
- :warning: In cazul in care placa nu are codul incarcat in memoria sa, este necesara urcarea prin apasarea butonului PlatformIO: Upload. Acesta este un script care compileaza sistemul, iar daca nu sunt erori, va sterge memoria placii si o va incarca cu noul cod dorit.
Dupa terminarea incarcarii, codul se va rula automat si terminalul de monitorizare se va deschide. De asemenea, pentru montarea fisierelor statice prin SPIFFS (daca nu au fost deja montate) se va accesa meniul PlatformIO: Upload Filesystem Image.

4. ESP-32 se va conecta atat la reteaua setata, cat si la cititorul OBD-II. Acesta va incepe prin setarea modului de coexistenta, conectarea la retea, furnizarea de IP local pentru accesarea interfetei, initializarea canalului de comunicare cu OBD-II, apoi va interoga parametrii setati. Progresul va fi afisat in Serial Monitor, iar parametrii vor fi afisati pe serverul web in timp real.

> ESP-32 indica faptul ca s-a reusit conectarea la retea (si furnizearea de IP) prin aprinderea unui LED integrat de culoarea albastra 