# 
# Copyright (c) 2016, Denis Vasilkovskii
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#      - Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      - Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      - Neither the name of the OINPE NRNU MEPhI nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# Tests/unit/run_tests.sh
#
# Created: 30-Oct-16 17:58:30
#  Author: Denis Vasilkovskii
#
#   About: Этот скрипт, написанный на языке shell последовательно запускает на 
#          исполнение все файлы, которые были ему переданы в качестве аргументов.
#          Если что-то пошло не так (хотя бы один файл вернул ненулевой статус),
#          то скрипт запоминает ошибку и возвращает этот статус в конце своей
#          работы.
#
#          Помимо запуска, скрипт также переопределяет вывод исполняемых файлов
#          в фалы с расширениями .out (для вывода stdout) и .err (для вывода
#          stderr). 

#!/bin/bash

output_result=0

for test_name in $@ ; do
	# извлекаем имя файла из переданного объекта
	filename=`basename ${test_name}`
	# выводим текст, но не заканичиваем его символом конца строки
	echo -n "Running ${filename}... "
	# собственно запуск теста
	${test_name} 1>${test_name}.out 2>${test_name}.err

	# забираем результат исполнения программы
	last_err=$?
	if [ $last_err -ne 0 ] ; then
		# если что-то пошло не так
		echo "Fail (${last_err})"
		# цитируем файл с ошибками в консоли
		cat ${test_name}.err
		# запоминаем результат
		output_result=$last_err
	else
		# когда всё ок
		echo "OK"
	fi
done

exit $output_result
