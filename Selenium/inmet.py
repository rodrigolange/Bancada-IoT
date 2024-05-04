from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
import time

driver = webdriver.Chrome()
driver.get("https://tempo.inmet.gov.br/TabelaEstacoes/A883")
time.sleep(3)


data = driver.find_elements(By.XPATH, '//tbody/tr/td[1]')
data1 = driver.find_elements(By.CLASS_NAME, 'titulo-data')

d1 = data1[0]

print(data)
print(d1.text)
