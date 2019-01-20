set -e
doxygen Doxyfile
doxybook -i doxygen/xml -o doxygen -t vuepress
