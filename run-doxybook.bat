doxygen Doxyfile || exit /b
doxybook -i doxygen-output/xml/index.xml -o . -s docs -dt docs/DOXYGEN_TEMPLATE.md -st docs/SUMMARY_TEMPLATE.md 
