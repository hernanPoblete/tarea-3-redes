#!/bin/bash


# Crea una copia de los archivos en Routes para tener mtu (me dio paja copiarlos uno por uno)
for i in 1 2 3 4 5
do

ext=".txt"
raw="./Routes/rutas_R$i""_v3"


new="$raw""_mtu""$ext"
old="$raw$ext" 

touch "$new"
cat "$old" > "$new"
done

# Para qué hacer una labor de 30 minutos pudiendo pasar 2 horas intentando fallidamente automatizarla? \j