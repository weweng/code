#!/usr/bin/env bash 

#  Wenwei Weng
#
# This script is to build OpenGrok index for the source code in the given directory
# After index is created, it will update the web, which is Tomcat based
# For GIT based repo, it will build history and enable annotation.
# 


if [ $# -lt  2 ]
then
  echo "Usage: Please specify as following: "
  echo ""
  echo " build-opengrok.sh SRC-DIR WEB-NAME ACTION OPENGROK-HOME OPENGROK-WS TOMCAT-HOME UNIVERSAL-CTAGS JAVA-HOME "
  echo ""
  echo "    SRC-DIR :      Full directory name which contains source code to be indexed "
  echo "    WEB-NAME:      name to be deployed under tomcat (war file name) "
  echo "    ACTION:        create or update "
  echo "    OPENGROK-HOME: OpenGrok install location(optional)"
  echo "                   default: /scratch/opengrok-rel-1.3.6 "
  echo "    OPENGROK-WS:   OpenGrok worksapce, i.e. location to store index(optional) "
  echo "                   default: /scratch/OPENGROK_WS "
  echo "    TOMCAT-HOME:   Tomcat install location (optional)"
  echo "                   default: /scratch/apache-tomcat-9.0.30"
  echo "    UNIVERSAL-CTAGS:  which ctags to use (optional)"
  echo "                   default: /usr/local/bin/ctags "
  echo "    JAVA-HOME:     Java home location (optional) "
  echo "                   default: /scratch/jdk-11.0.5  "
  echo ""
  echo ""
  echo "    Example: "
  echo "     build-opengrok.sh /scratch/my-code-repo mycode create /scratch/opengrok-rel-1.3.6 /scratch/OPENGROK_WS /scratch/apache-tomcat-9.0.30 /usr/local/bin/ctags /scratch/jdk-11.0.5 "
  echo " "

  exit
else
  DIRLOCATION=$1
  WEBAPPNAME=$2
  OPENGROK_WEBAPP_CONTEXT=$2

  if [ $# -gt  2 ]
  then
     ACTION=$3
  else
     ACTION="create"
  fi

  if [ $# -gt  3 ] 
  then
     OPENGROK_HOME=$4
  else 
     OPENGROK_HOME="/scratch/opengrok-rel-1.3.6"
  fi
  if [ $# -gt  4 ]
  then
     OPENGROK_WS_BASE=$5
  else 
     OPENGROK_WS_BASE="/scratch/OPENGROK_WS" 
  fi
  if [ $# -gt  5 ]
  then
     OPENGROK_TOMCAT_BASE=$6 
  else
     OPENGROK_TOMCAT_BASE="/scratch/apache-tomcat-9.0.30"
  fi
  if [ $# -gt  6 ]
  then
     UNIVERSAL_CTAGS=$7
  else
      UNIVERSAL_CTAGS="/usr/local/bin/ctags"
  fi 
  if [ $# -gt  7 ]
  then
     JAVA_HOME=$8
  else
     JAVA_HOME="/scratch/jdk-11.0.5"
  fi 
fi

OPENGROK_DISTRIBUTION_BASE=$OPENGROK_HOME/lib

OPENGROK_APP_SERVER="Tomcat" 

OPENGROK_WAR_TARGET_TOMCAT=${OPENGROK_TOMCAT_BASE}/webapps 

OPENGROK_VERBOSE=1
OPENGROK_SCAN_DEPTH=24

OPENGROK_DIST_WAR=$OPENGROK_DISTRIBUTION_BASE/source.war

HOST_NAME=`hostname`
HTTP_PORT="8080"

# Preparing to build a new OpenGrok instance

DIR_NAME=$WEBAPPNAME
WORK_DIR=`echo ${OPENGROK_WS_BASE}/${DIR_NAME}`
WORK_DIR_DATA=`echo ${OPENGROK_WS_BASE}/${DIR_NAME}/DATA`
WORK_DIR_DATA_data=`echo ${OPENGROK_WS_BASE}/${DIR_NAME}/DATA/data`
WORK_DIR_DATA_etc=`echo ${OPENGROK_WS_BASE}/${DIR_NAME}/DATA/etc`

if [ $ACTION == "create" ]; then
    # clean it up if it exists
    if [ -d ${WORK_DIR} ]; then
        rm -rf ${WORK_DIR}
    fi

    mkdir -p $WORK_DIR_DATA_data
    mkdir -p $WORK_DIR_DATA_etc

    # Create softlink for OpenGrok to use
    cd $WORK_DIR_DATA
    ln -s $DIRLOCATION src
fi

##########################################################################
# Now we have raw data prepared and ready to build 
# Let's set up more env for this branch specifically

# this branch's instance, e.g. for SRC_ROOT, SRC_DATA
OPENGROK_INSTANCE_BASE=$WORK_DIR_DATA

# this is to tell we want to have webapp name to be consistent 
# with indexer (by default is source)
OPENGROK_WEBAPP_CONTEXT=$WEBAPPNAME

# Build OpenGrok Index
if  [ $ACTION == "create" ]; then
    echo -e "\n Building OpenGrok index for $WEBAPPNAME ..."
else
    echo -e "\n Updating OpenGrok index for $WEBAPPNAME ..."
fi
date

echo  -e "\n $JAVA_HOME/bin/java -Xmx2048m \
    -Djava.util.logging.config.file=$OPENGROK_HOME/doc/logging.properties \
    -jar $OPENGROK_HOME/lib/opengrok.jar \
    -c $UNIVERSAL_CTAGS \
    -s $WORK_DIR_DATA/src -d $WORK_DIR_DATA -H -S -G \
    -W $WORK_DIR_DATA_etc/configuration.xml \
    --leadingWildCards on"

$JAVA_HOME/bin/java -Xmx2048m \
    -Djava.util.logging.config.file=$OPENGROK_HOME/doc/logging.properties \
    -jar $OPENGROK_HOME/lib/opengrok.jar \
    -c $UNIVERSAL_CTAGS \
    -s $WORK_DIR_DATA/src -d $WORK_DIR_DATA -H -S -G \
    -W $WORK_DIR_DATA_etc/configuration.xml \
    --leadingWildCards on

echo -e "\n\n Generated instance configuration.xml file to be packed in war file \n\n " 
ls -l $WORK_DIR_DATA_etc/configuration.xml
 
# Update OpenGrok web

# First shut down the webserver
echo " Shutdown Tomcat server first ..."
$OPENGROK_TOMCAT_BASE/bin/shutdown.sh > /dev/null
echo ""

echo " Update OpenGrok web link for $WEBAPPNAME ..."
cd ${OPENGROK_TOMCAT_BASE}
if [ -f ${WEBAPPNAME}.war ]; then
    rm ${WEBAPPNAME}.war
fi

if [ -d ${WEBAPPNAME} ]; then
    rm -rf ${WEBAPPNAME}
fi

# update web using OpenGrok wrapper/script
echo " Deploy $WEBAPPNAME from $OPENGROK_INSTANCE_BASE in OpenGrok ..."
#$OPENGROK_HOME/bin/OpenGrok deploy $WEBAPPNAME
TMP_WORK_DIR="temp_war_opengrok"

cd ${OPENGROK_INSTANCE_BASE}/..

# Create a customized war file pointing to the generated DATA directory
if [ -d $TMP_WORK_DIR ]; then 
    rm -rf $TMP_WORK_DIR
fi
mkdir $TMP_WORK_DIR
cd $TMP_WORK_DIR

$JAVA_HOME/bin/jar -xf $OPENGROK_DIST_WAR

# update the WEB-INF/web.xml file
sed -i "s|\/var\/opengrok\/etc|$WORK_DIR_DATA_etc|g" WEB-INF/web.xml

if [ -f ../${WEBAPPNAME}.war ]; then 
    rm ../${WEBAPPNAME}.war
fi
    
$JAVA_HOME/bin/jar -cf $WORK_DIR/${WEBAPPNAME}.war *

cd ..
rm -rf $TMP_WORK_DIR

# push into tomcat space
cp $WORK_DIR/${WEBAPPNAME}.war $OPENGROK_WAR_TARGET_TOMCAT/

echo " Start Tomcat server..."
#Start webserver again
$OPENGROK_TOMCAT_BASE/bin/startup.sh

echo "Please add a link under Tomcat home page for http://${HOST_NAME}:${HTTP_PORT}/$WEBAPPNAME"

