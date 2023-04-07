TODO:
* yarismanin ortasinda music aktiflestirilirse de yap. basa almak zorunda kalinmasin
* about, update vs action'ları ekle
* about'a icon'ın source'unu koy
* geri dönüş değeri bool olan gibi fonksiyonların bilgilerini kullan, hata olması durumunda yazdır.
* db boşken yarışma başlatılırsa uyarı ver
* sandbox'ta qt.tlsbackend.ossl: Failed to load libssl/libcrypto. hatasi verdi. 
* sandbox'ta shortcut'ta icon gorunmuyor. halbuki refer ettigi exe'de icon var
* sandbox'ta contentAccessDenied hatası verdi, baktım ana makinada da aynı hata. api.json'da günlük api sınırını aştınız yazıyodu. kısa süre sonra gitti nedense. edit: yine verdi.
* yeni sürümü indirirken şimdi protocolunknown error verdi. edit: sonra vermedi
* birden fazla yarışma eklenebilir olsun. mesela gençler için ayrı, çocuklar için ayrı

Paketleme öncesinde yapılacaklar:
* .exe'yi bir klasöre alıp şunu yap: windeployqt.exe bilgiyarismasi.exe -no-translations --no-opengl-sw --no-system-d3d-compiler
* version.txt'yi güncelle.
* inno setup'taki versiyon bilgisini güncelle ve inno setup scriptini çalıştır