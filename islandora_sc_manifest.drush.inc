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

  require_once "includes/MyFedoraObject.inc";
  require_once "includes/SharedCanvasManifest.inc";
  require_once "includes/Canvas.inc";

  $base_url = "https://paleography.library.utoronto.ca";
  $dsid = "JPGHIRES";

  $fo = new MyFedoraObject();

  $object = $fo->get_object($pid);

  $manifest = new SharedCanvasManifest("$base_url/islandora/object/$object_pid/datastream/SC/view", $object->label, "$base_url/islandora/object/$object_pid/datastream/MODS");
  
  $pages = $fo->get_pages($pid);
  echo "Processing $pid\n";
  
  foreach($pages as $page) {
      $page_pid = $page['pid']['value'];
      echo "Processing Page: $page_pid\n";
      $page_object = $fo->get_object($page_pid);
      $page_uri = "$base_url/islandora/object/$page_pid";
      
      $page_object_rels = $page_object['JP2']->relationships;
      $width_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'width');
      $height_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'height');
      $width = $width_rel[0]['object']['value'];
      $height = $height_rel[0]['object']['value'];

      // this width and height is for the original JP2
      // recalculate width and height based on fixed height of 2000 (which is the case for JPGHIRES
      $height = round($width * 2000/$height);
      $width = 2000;
      
      $mimetype = $page_object[$dsid]->mimetype;
      
      //echo $mimetype . "\n";
      //echo $width . "\n";
      //echo $height . "\n";

      $canvas = new Canvas($page_uri, $page_pid, $page_object->label, $page_uri."/datastream/MODS/view");
      $canvas->setImage($page_uri."/datastream/$dsid/view", $mimetype, $width, $height);
      $manifest->addCanvas($canvas);

      //print_r($canvas);
      
      //break;
  }
  
  //break;
  $json = $manifest->getJson();
  $fo->add_datastream(&$object, "SC", $json);
  drupal_set_message(t('SC Manifest created'));
}