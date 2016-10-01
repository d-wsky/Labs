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
# Tests/unit/Makefile
#
# Created: 01-Oct-16 21:45:33
#  Author: Denis Vasilkovskii
#
# Disclaimer: All comments are in Windows CP1251 (not UTF8) to guarantee support
# for all make versions.

# уровень вложени€ до корневой директории
ROOT_PATH=../..
# включаем файл со входными настройками
include $(ROOT_PATH)/Makefile.in

# названи€ юнит-тестов, которые будут скомпилированы
test_names = fsm_linear

# исключаем флаг "только компил€ци€", чтобы на выходе был исполн€емый файл
CFLAGS := $(filter-out -c,$(CFLAGS))

# с помощью этой строки названи€ юнит-тестов преврат€тьс€ в названи€ исполн€емых
# файлов, которые будут собраны
objects = $(addsuffix $(EXTENSION),$(addprefix $(OUTPUT_DIR)/,$(test_names)))

# файлы и директории, которые создаютс€ в процессе сборки и тестировани€
clean_files = $(OUTPUT_DIR)

# дополнительные флаги линковщика позвол€ют перегружать функции библиотеки внутри тестов
LDFLAGS += -Xlinker --allow-multiple-definition

# с помощью этой цели стро€тс€ все юнит-тесты
all: build_directories $(objects)

# с помощью этой цели создаетс€ директори€ дл€ исполн€емых файлов
build_directories:
	$(Q)$(MK) $(OUTPUT_DIR)

# с помощью этой цели удал€ютс€ все временные и выходные файлы. ‘лаг PHONY говорит
# утилите Make, что в результате исполнени€ этой цели не создаетс€ новых файлов
.PHONY: clean
clean:
	$(Q)$(RM) $(clean_files)

# с помощью этой цели создаютс€ исполн€емые файлы юнит-тестов
$(OUTPUT_DIR)/%$(EXTENSION): %.c
	@echo CC $(notdir $*)
	$(Q)$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(LDFLAGS) $^ -o $@ -l$(LIB_NAME)
