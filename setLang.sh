if [ -z ${1+x} ];
then
  echo "Bonjour, vous allez pouvoir choisir votre langage préféré. Relancez ce script avec en paramètre, le chiffre correspondant au langage souhaité. ";
  echo "0 : Français";
  echo "1 : English";
else
  echo "#define LANG $1" > src/internationalization/conf.h
  echo "Votre langage préféré a été mis à jour. ";
fi
