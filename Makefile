.PHONY: test gcov_report

MAKE_TEST = Makefile.test

all:
	cd ./qt_calc  && qmake && $(MAKE) -f Makefile.Release all
	
install:
	cd ./qt_calc  && qmake && $(MAKE) -f Makefile.Release install
	
uninstall:
	cd ./qt_calc  && qmake && $(MAKE) -f Makefile.Release uninstall

clean:
	cd ./qt_calc  && qmake && $(MAKE) clean
	-rm *.tar
	$(MAKE) -f $(MAKE_TEST) clean

dvi:
	@echo "BIP......... BIP............. BIP..........."
	@echo "Generating documentation"
	@sleep 1
	@echo "10%"
	@sleep 1
	@echo "30%"
	@sleep 1
	@echo "60%"
	@sleep 1
	@echo "80%"
	@sleep 1
	@echo "90%"
	@sleep 1
	@echo "99%"
	@sleep 1
	@echo "100%"
	@echo "BINGOOO! Your documentation is created at README.md! Just open it and recieve your kaif, good luck!"

dist:
	tar -cvf archive.tar images qt_calc Makefile README.md

test: 
	@echo ok
	$(MAKE) -f $(MAKE_TEST) test

gcov_report:
	$(MAKE) -f $(MAKE_TEST) gcov_report
