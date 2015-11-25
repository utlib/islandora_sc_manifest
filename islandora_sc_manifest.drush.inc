<?php

function islandora_sc_manifest_drush_command() {

  return array(
    'sc-manifest-generator' => array(
      'description' => 'Generate SC manifest for the book object',
      'aliases' => array('scgen'),
      'arguments'   => array(
        'pid'    => dt('PID_TO_REINDEX|full'),
      ),
    ),
  );
}

function drush_islandora_sc_manifest_sc_manifest_generator($pid = NULL) {
  module_load_include('inc', 'islandora_sc_manifest', 'includes/MyFedoraObject');
  module_load_include('inc', 'islandora_sc_manifest', 'includes/SharedCanvasManifest');
  module_load_include('inc', 'islandora_sc_manifest', 'includes/Canvas');
  module_load_include('inc', 'islandora_sc_manifest', 'includes/utilities');

  if (empty($pid)) {
    return drush_print("Please provide PID or full as the argument");
  }

  if ($pid != 'full') {
    generate_sc_manifest($pid, TRUE);
  } else {
    $fo = new MyFedoraObject();
    $manuscripts = $fo->get_children('paleography:manuscripts');
    foreach ($manuscripts as $manuscript) {
      $pid = $manuscript['pid']['value'];
      generate_sc_manifest($pid,TRUE);
    }
  }
  

  
}