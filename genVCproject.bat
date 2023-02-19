@rem #change Directory


@rem change Driver letter
@%~d0

@rem change working direcotory
@cd %~dp0



mkdir _build
cmake -B _build .

@pause