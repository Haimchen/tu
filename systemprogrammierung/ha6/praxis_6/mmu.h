#ifndef MMU_H
#define MMU_H

#define PAGE_COUNT (2048)
#define PAGE_SIZE (16)

enum error_enum
{
	PAGE_UNAVAILABLE = -1,
	INVALID_PHYSICAL_ADDRESS = -2,
	RIGHTS_MISSMATCH = -3,
    UNIMPLEMENTED = -4
};
typedef enum error_enum error;

enum access_type_enum
{
	READ,
	WRITE
};
typedef enum access_type_enum access_type;

enum sys_mode_t_enum
{
	KERNEL,
	USER
};
typedef enum sys_mode_t_enum sys_mode_t;


void switch_mode(sys_mode_t new_mode);
// Simuliert den Wechsel vom Kernelnutzer zum Anwendernutzer sowie den Wechsel zurück.


void init_page_table(void);
// Initialisiert die Seitentabelle. Dabei wird eine eins zu eins
// Zuordnung realisiert. Sämtliche Seiten sind anschließend auch im
// Anwendermodus zugreifbar und sowohl lesend als auch schreibend nutzbar.
// Diejenigen Seiten, welche ausschließlich auf physikalische Adressen in
// einem der freien Block (siehe Speicherlayout) umgesetzt werden,
// sind unverfügbar/invalide.
// Es wird bei keiner der Seiten eine der angegebenen Information ausgelassen.
// So werden beispielsweise die Bits der Kachel auch dann initialisiert,
// wenn die Seite eigentlich unverfügbar ist.


void remap(int16_t page, int16_t tile);
// Ordnet der Seite mit dem Index "page" der Kachel mit dem Index "tile" zu.
// Es ist zu beachten, dass es sich bei den Werten, welche "page" und "tile"
// beinhalten, nicht um Adressen, sondern um Indizes handelt. Die höchstwertigen
// 5 Bits der Parameter "page" und "tile" werden daher ignoriert.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void close_page(int16_t page);
// Macht die angegebene Seite mit dem Index "page" unverfügbar.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void open_page(int16_t page);
// Macht die angegebene Seite mit dem Index "page" verfügbar.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void page_kernel_only(int16_t page);
// Schränkt den Zugriff auf die angegebene Seite mit dem Index "page"
// auf den Kernelmodus ein.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void open_page_for_user(int16_t page);
// Weitet den Zugriff auf die angegebene Seite mit dem Index "page"
// auf den Anwendermodus aus.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void page_read_only(int16_t page);
// Auf die angegebene Seite mit dem Index "page" kann anschließend
// nur noch lesend zugegriffen werden.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


void page_read_write(int16_t page);
// Auf die angegebene Seite mit dem Index "page" kann anschließend
// sowohl lesend als auch schreibend zugegriffen werden.
// Die höchstwertigen 5 Bits des Parameters "page" werden ignoriert.
// Alle anderen Informationen des entsprechenden Seitentabelleneintrags bleiben
// gleich.
// Diese Operation darf nur im Kernelmodus ausgeführt werden und führt im
// Anwendermodus zu keiner Veränderung.


int16_t memory_access(int16_t address, access_type operation);
// Simuliert den indirekten Zugriff auf den Speicher über eine MMU.
// Der Parameter "address" gibt die virtuelle Adresse an, auf welche
// zugegriffen werden soll. Die Adressbreite entspricht per Definition 15 Bit.
// Daher wird das höchstwertige Bit des Parameters "address" ignoriert.
// Der Parameter "operation" gibt an, ob lesend oder schreibend zugegriffen
// werden soll.
// Der Rückgabewert gibt die physikalische Adresse an, auf welche zugegriffen
// wird, sofern es zu keinem Fehler kommt. Fehler werden durch negative
// Rückgabewerte gekennzeichnet. Die möglichen Fehler sowie die zugehörigen
// Werte sind im Datentyp "error" festgehalten.
// Wird versucht, auf eine physikalische Adresse zuzugreifen, welche nicht
// verdrahtet ist ("frei" im Speicherlayout), so wird stattdessen
// "INVALID_PHYSICAL_ADDRESS" zurückgegeben.
// Wird auf eine unverfügbare Seite zugegriffen, so wird stattdessen
// "PAGE_UNAVAILABLE" zurückgegeben.
// Wird im Anwendermodus versucht, auf eine für den Kernel exklusive Seite
// zuzugreifen, so wird stattdessen "RIGHTS_MISSMATCH" zurückgegeben.
// Wird versucht, schreibend auf eine Seite zuzugreifen, welche lediglich
// gelesen werden darf, so wird stattdessen "RIGHTS_MISSMATCH" zurückgegeben.


void protect_system(void);
// Es wird angenommen, dass das Betriebssystem die ersten 4096 Bytes des
// RAMs einnimmt. Dieser Bereich ist nach Ausführung dieser Funktion vor
// Zugriffen im Anwendermodus geschützt.
// Es wird außerdem angenommen, dass Ein- und Ausgabegeräte ausschließlich
// vom Betriebssystem direkt angesprochen werden dürfen. Daher sind auch diese
// nach Ausführung dieser Funktion vor Zugriffen im Anwendermodus geschützt.

#endif // MMU_H

