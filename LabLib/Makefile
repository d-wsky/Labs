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
# LabLib/Makefile
#
# Created: 01-Oct-16 21:24:28
#  Author: Denis Vasilkovskii
#
# Disclaimer: All comments are in Windows CP1251 (not UTF8) to guarantee support
# for all make versions.

# уровень вложения до корневой директории
ROOT_PATH=..
# включаем файл со входными настройками
include $(ROOT_PATH)/Makefile.in
# итоговое имя файла библиотеки
LIB_FILE=$(OUTPUT_DIR)/lib$(LIB_NAME).a

# списки файлов, которые будут скомпилированы при исполнении цели all
common += fsm
common += $(LIB_NAME)

pc += libtime
pc += interrupts

# с помощью этой строчки списки common и pc превращаются в названия файлов с относительными путями
objects_common = $(addsuffix .o,$(addprefix $(OUTPUT_DIR)/,$(addprefix common/,$(common))))
objects_pc = $(addsuffix .o,$(addprefix $(OUTPUT_DIR)/,$(addprefix pc/,$(pc))))

# описание того, какие файлы создаются во время сборки библиотеки
clean_files = $(OUTPUT_DIR)

# все файлы, которые надо собрать до того, как собрать библиотеку
all_objects = $(objects_common) $(objects_pc)

all: build_out_dir $(LIB_FILE)

build_out_dir: 
	$(Q)$(MK) $(OUTPUT_DIR)
	$(Q)$(MK) $(OUTPUT_DIR)/common
	$(Q)$(MK) $(OUTPUT_DIR)/pc

# с помощью этой цели будет собрана библиотека из отдельных объектных файлов
$(LIB_FILE): $(all_objects)
	$(Q)echo AR $(notdir $@)
	$(Q)$(AR) $(AR_KEYS) $(LIB_FILE) $(all_objects)

# с помощью этой цели собираются файлы из папки common
$(OUTPUT_DIR)/common/%.o: common/%.c
	$(Q)echo CC $(notdir $@)
	$(Q)$(CC) $(CFLAGS) $^ -o $@ 

# с помощью этой цели собираются файлы из папки pc
$(OUTPUT_DIR)/pc/%.o: pc/%.c
	$(Q)echo CC $(notdir $@)
	$(Q)$(CC) $(CFLAGS) $^ -o $@ 

# цель очистки временных и выходных файлов. Имеет префикс PHONY, показывающий
# утилите make, что никаких файлов в результате исполнения этой цели не создается
.PHONY: clean
clean:
	$(Q)$(RM) $(clean_files)
