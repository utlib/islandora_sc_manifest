<?php

function islandora_sc_manifest_drush_command() {

  return array(
    'sc-manifest-generator' => array(
      'description' => 'Generate SC manifest for the book object',
      'aliases' => array('scgen'),
      'arguments' => array(
        'pid' => dt('PID_TO_REINDEX|full'),
        'debug' => dt('Debug mode: true|false'),
      )
    )
  );
}

function drush_islandora_sc_manifest_sc_manifest_generator($pid = NULL, $debug = FALSE) {

  if (!libraries_load('utlfedoraobject') and !module_load_include('inc', 'islandora_sc_manifest', 'includes/utlfedoraobject'))
    return drush_print("UTLFedoraObject not found!");

  if (empty($pid)) return drush_print("Please provide PID or full as the argument");

  require_once ("includes/utilities.inc");

  if ($debug) drush_print("Running in DEBUG mode.\n");

  if ($pid != 'full') {
    __generate_iiif_manifest($pid, TRUE, $debug);
  } else {
    echo "Full option doesn't work at the moment. Please specify a sinlge PID.\n";
    $user = user_load(1);
    $fo = new UTLFedoraObject($user->name, $user->pass);
    $manuscripts = $fo->get_children('paleography:manuscripts');
    foreach ($manuscripts as $manuscript) {
      $pid = $manuscript['pid']['value'];
      echo "$pid\n";
      // __generate_iiif_manifest($pid, TRUE, $debug);
    }
  }
  
}
