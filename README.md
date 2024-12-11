<h1>Járműfedélzeti Rendszerek 2. PROJEKTFELADAT</h1>
A feladat a 2024/25 I. félév során készült <br>
A feladatot készítették: Bakonyvári Bence, Hódi Balassa, Simon Zalán <br>
Gyakorlatvezető: Dr. Fehér Árpád <br>
Előadó: Dr. Aradi Szilárd

<h2>Feladat: </h2>
<p>Óra, dátum, hőmérséklet kiolvasás, beállítás</p>
<ul>
  <li>Készítsék el a BIGAVR6 boardon található valós idejű óra IC-vel és hőmérséklet szenzorral való kommunikáció megvalósításához szükséges függvényeket</li>
  <li>Jelenítsék meg az alfanumerikus LCD kijelzőn a kiolvasott aktuális időt, dátumot és hőmérsékletet, és implementáljanak beállítási funkciókat, amelyekkel a felhasználóbarát módon gombokkal lehet módosítani az időt és dátumot. </li>
  <li>1 Hz frekvenciával küldjék el az idő, dátum és hőmérséklet adatokat a CAN hálózatra a mellékelt .dbc alapján.</li>
  <li>Az egyes szoftvermodulokat szervezzék ki külön .c fájlba. </li>
</ul>

<h2>Feladat megvalósításának lépései:</h2>
<ol>
  <li>
    Valós idejű óra IC-vel való kommuniáció megvalósítása
    <ul>
      <li>Adatok írása, fogadása I2C-n keresztül (év, hónap, nap, óra, perc, másodperc)</li>
    </ul>
  </li>
  <li>
    Hőmérő IC-vel való kommunikáció megvalósítása
    <ul>
      <li>Adat irás, fogadás egyvezetékes buszrendszeren keresztül (hőmérséklet) </li>
    </ul>
  </li>
  <li>
    Gombokkal történő felhasználóbarát kezelőfelület megvalósítása
    <ul>
      <li>Gombnyomás logika a dátum és idő szerkesztéséhez, elmentéséhez, az értékek megváltoztatásához</li>
      <li>Megfelelő értékek megjelenítése az LCD kijelzőn</li>
    </ul>
  </li>
  <li>
    CAN kommunikáció megvalósítása
    <ul>
      <li>Hőmérséklet, dátum és idő adatok kiküldése</li>
    </ul>
  </li>
</ol>

<h2>Feladatban szereplő eszközök:</h2>
<ol>
  <li>AT90CAN128: Mikrovezérlő</li>
  <li>BIGAVR6: Fejlesztői panel</a></li>
  <li>DS1307: Valós idejű óra IC</li>
  <li>DS18S20: Hőmérséklet IC</li>
  <li>WH1602B: LCD kijelző</li>
  <li>ST7066U: LCD kijelző vezérlő IC</li>
</ol>

<h2>GITHUB Repo áttekintése</h2>
Az egyes mappák tartalmai: 
<ul>
  <li>
    C template: Tantárgyi minták a .c és .h típusú file-ok belső felépítéséhez
  </li>
  <li>
    Datasheets: A 'Feladatban szereplő eszközök' szakaszban szereplő IC-k datahseet-jei
  </li>
  <li>
    Feladatkiírás: A feladatkiírás megfogalmazása, a saját jegyzeteink a datasheet-ekről, illetve a feladat megvalósításához
  </li>
  <li>
    <strong>Microchip Studio: A mikrovezérlőre rátöltött VÉGLEGES script-ek</strong>
  </li>
  <li>
    Órai anyagok: A félév során az órai munkát script-jei
  </li>
  <li>
    Scriptek: A kiindulási scriptek, amikből elkezdve oldottuk meg a feladatot, írtuk meg a saját programunkat
  </li>
</ul>
