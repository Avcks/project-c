$content = Get-Content -Raw "d:\c project\14\main12_2.c"

$macro = @"
/* ══════════════════ INPUT SAFEGUARDS ══════════════════ */
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

/* ── credentials ── */
"@

$content = $content -replace '/\* ── credentials ── \*/', $macro

$content = [regex]::Replace($content, 'scanf\("%d",\s*&([a-zA-Z0-9_\[\]\.]+)\);', 'SAFE_INPUT_INT($1);')
$content = [regex]::Replace($content, 'scanf\("%f",\s*&([a-zA-Z0-9_\[\]\.]+)\);', 'SAFE_INPUT_FLOAT($1);')

Set-Content -Path "d:\c project\14\main12_2.c" -Value $content -Encoding UTF8
Write-Host "Replaced successfully"
