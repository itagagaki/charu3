NAME=Charu3_$(VERSION)

dist:
	@echo Generatiing $(NAME).zip from `grep ProductVersion Charu3.rc|awk '{print $3}'`
	rm -f $(NAME).zip
	mkdir $(NAME)
	cp -r Release/Charu3.exe _locale *.bmp *.wav 00MANIFEST.txt README.md $(NAME)
	zip -r $(NAME).zip $(NAME)
