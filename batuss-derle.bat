@echo off
echo === Batuss Build Sistemi - TurkOS Derleyici (Windows) ===
echo.
where wsl >nul 2>nul
if %errorlevel% neq 0 (
    echo HATA: WSL bulunamadi.
    echo TurkOS derlemek icin Windows Subsystem for Linux (WSL2) gereklidir.
    echo Kurulum icin PowerShell'i yonetici olarak acip su komutu calistirin:
    echo     wsl --install
    echo Kurulumdan sonra bilgisayari yeniden baslatin ve bu dosyayi tekrar calistirin.
    pause
    exit /b 1
)
echo WSL bulundu, derleme WSL icinde baslatiliyor...
wsl bash -c "cd $(wsl wslpath '%~dp0') && chmod +x batuss-derle.sh && ./batuss-derle.sh %*"
pause
