# Fontes: https://github.com/rafaelpsdv/INMET-Scrap/blob/main/spider_inmet.py


from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from pyvirtualdisplay import Display
import time
import random
import pandas as pd

display = Display(visible=0, size=(1024, 768))
display.start()


options = webdriver.ChromeOptions()
options.add_argument('--no-sandbox')
options.add_argument('--disable-gpu')

driver = webdriver.Chrome(options=options)
driver.set_page_load_timeout(90)

driver.implicitly_wait(6)
driver.get("https://tempo.inmet.gov.br/TabelaEstacoes/A883")
time.sleep(5)

titulo = driver.find_elements(By.CLASS_NAME, 'titulo-data')

data = driver.find_elements(By.XPATH, '//tbody/tr/td[1]')
hora = driver.find_elements(By.XPATH, '//tbody/tr/td[2]')
temp_inst = driver.find_elements(By.XPATH, '//tbody/tr/td[3]')
temp_max = driver.find_elements(By.XPATH, '//tbody/tr/td[4]')
temp_min = driver.find_elements(By.XPATH, '//tbody/tr/td[5]')
umidade_inst = driver.find_elements(By.XPATH, '//tbody/tr/td[6]')
umidade_max = driver.find_elements(By.XPATH, '//tbody/tr/td[7]')
umidade_min = driver.find_elements(By.XPATH, '//tbody/tr/td[8]')
pto_orvalho_inst = driver.find_elements(By.XPATH, '//tbody/tr/td[9]')
pto_orvalho_max = driver.find_elements(By.XPATH, '//tbody/tr/td[10]')
pto_orvalho_min = driver.find_elements(By.XPATH, '//tbody/tr/td[11]')
pressao_inst = driver.find_elements(By.XPATH, '//tbody/tr/td[12]')
pressao_max = driver.find_elements(By.XPATH, '//tbody/tr/td[13]')
pressao_min = driver.find_elements(By.XPATH, '//tbody/tr/td[14]')
vento_vel = driver.find_elements(By.XPATH, '//tbody/tr/td[15]')
vento_dir = driver.find_elements(By.XPATH, '//tbody/tr/td[16]')
vento_raj = driver.find_elements(By.XPATH, '//tbody/tr/td[17]')
radiacao = driver.find_elements(By.XPATH, '//tbody/tr/td[18]')
chuva = driver.find_elements(By.XPATH, '//tbody/tr/td[19]')

datas = []
for i in range(len(hora)):
	temporary_data = {'hora':hora[i].text,
        	        'temp_inst':temp_inst[i].text,
                        'temp_max':temp_max[i].text,
                        'temp_min':temp_min[i].text,
                        'umidade_inst':umidade_inst[i].text,
                        'umidade_max':umidade_max[i].text,
                        'umidade_min':umidade_min[i].text,
                        'pto_orvalho_inst':pto_orvalho_inst[i].text,
                        'pto_orvalho_max':pto_orvalho_max[i].text,
                        'pto_orvalho_min':pto_orvalho_min[i].text,
                        'pressao_inst':pressao_inst[i].text,
                        'pressao_max':pressao_max[i].text,
                        'pressao_min':pressao_min[i].text,
                        'vento_vel':vento_vel[i].text,
                        'vento_dir':vento_dir[i].text,
                        'vento_raj':vento_raj[i].text,
                        'radiacao':radiacao[i].text,
                        'chuva':chuva[i].text
                        }
	datas.append(temporary_data)

jsonText = pd.DataFrame(datas).to_json(orient = 'columns')
print(jsonText)

