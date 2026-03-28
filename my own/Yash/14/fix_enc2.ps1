$content = Get-Content -Raw -Path "d:\c project\14\my.c" -Encoding UTF8

$macro = @"
/* ================== INPUT SAFEGUARDS ================== */
#define SAFE_INPUT_INT(var) do { \
    while (scanf("%d", &(var)) != 1) { \
        int _c; while ((_c = getchar()) != '\n' && _c != EOF); \
        printf("  [!] Invalid format. Please enter numbers: "); \
    } \
} while(0)

#define SAFE_INPUT_FLOAT(var) do { \
    while (scanf("%f", &(var)) != 1) { \
        int _c; while ((_c = getchar()) != '\n' && _c != EOF); \
        printf("  [!] Invalid format. Please enter numbers: "); \
    } \
} while(0)

`$1
"@

$content = [regex]::Replace($content, '(/\*.*credentials.*\*/)', $macro)
$content = [regex]::Replace($content, 'scanf\("%d",\s*&([a-zA-Z0-9_\[\]\.]+)\);', 'SAFE_INPUT_INT($1);')
$content = [regex]::Replace($content, 'scanf\("%f",\s*&([a-zA-Z0-9_\[\]\.]+)\);', 'SAFE_INPUT_FLOAT($1);')

[System.IO.File]::WriteAllText("d:\c project\14\main12_3.c", $content, [System.Text.Encoding]::UTF8)
Write-Host "Replaced successfully and saved as main12_3.c"
