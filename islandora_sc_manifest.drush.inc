<?php

function islandora_sc_manifest_drush_command() {

  return array(
    'sc-manifest-generator' => array(
      'description' => 'Generate the IIIF representation for the object',
      'aliases' => array('iiifgen'),
      'arguments' => array(
        'pid' => dt('PID of the object to generate IIIF')
      ),
      'options' => array(
        'recursive' => dt('Whether to recursively generate iiif for this object including its children: true|false. Defaults to false.'),
        'test' => dt('Wheter to run the command in Test mode: true|false. Defaults to false.'),
        'show' => dt('Wheter to print the resulting IIIF JSON object: true|false. Defaults to false.')
      )
    )
  );
}

function drush_islandora_sc_manifest_sc_manifest_generator($pid=NULL) {

  if (!libraries_load('utlfedoraobject') and !module_load_include('inc', 'islandora_sc_manifest', 'includes/utlfedoraobject'))
    return drush_print("UTLFedoraObject not found!");

  if (empty($pid)) return drush_print("Please provide PID for the object as the argument");

  require_once ("includes/utilities.inc");

  $test = drush_get_option('test', FALSE);
  $recursive = drush_get_option('recursive', FALSE);
  $show = drush_get_option('show', FALSE);

  if ($test) drush_print("\n<<<<<<<-- Running in TEST mode. This will not ingest to Fedora. -->>>>>>>>\n");
  
  __generate_iiif_manifest($pid, TRUE, $test, $show);

  // Recursively generate IIIF for children objects.
  if ($recursive) {
    echo "\n<<<<<<<-- Running in Recursive mode. -->>>>>>>>\n\n";
    $user = user_load(1);
    $fo = new UTLFedoraObject($user->name, $user->pass);
    $object = $fo->get_object($pid);
    if (in_array('islandora:collectionCModel', $object->models)){
      $children = $fo->get_collection_children($pid); // Get all the children objects of this Collection
    } else { // 'islandora:collectionCModel'
      $children = $fo->get_compound_children($pid); // Get all the children objects of this Compound Object
    }
    foreach ($children as $child) {
      $pid = $child['pid']['value'];
      __generate_iiif_manifest($pid, TRUE, $test, $show);
      echo "\n";
    }
  }
    
  
}
