<?php

function islandora_sc_manifest_drush_command() {

  return array(
    'sc-manifest-generator' => array(
      'description' => 'Generate SC manifest for the book object',
      'aliases' => array('scgen'),
      'arguments'   => array(
        'pid'    => dt('PID'),
      ),
    ),
  );
}

function islandora_sc_manifest_drush_help($command) {
  switch ($command) {
      case 'drush:scgen':
        return dt('drush scgen pid');
  }  
}

function drush_islandora_sc_manifest_sc_manifest_generator($pid = NULL) {
  if (empty($pid)) {
    return drush_print("Please provide PID as the argument");
  }

  module_load_include('inc', 'islandora_sc_manifest', 'includes/utilities');

  generate_sc_manifest($pid, TRUE);
}