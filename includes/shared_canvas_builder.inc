<?php

/**
 * @file
 * Shared canvas builder functionality.
 */

function islandora_sc_get_shared_canvas_manifest(AbstractObject $object) {
  global $base_url;
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  $pages = islandora_paged_content_get_pages($object);
  $manifest = islandora_sc_get_book_object_manifest($object);
  foreach ($pages as $page => $info) {
    $page_object = islandora_object_load($page);
    $page_uri = "$base_url/islandora/object/$page";
    $jp2_view = url("islandora/object/{$page}/datastream/JP2/view", array(
      'absolute' => TRUE,
      'query' => array(
        'token' => islandora_get_object_token($page, 'JP2', 1),
      ),
    ));
   $manifest['sequences'][0]['canvases'][] = islandora_sc_get_page_object_manifest($page_object, $info, $jp2_view); 
  }
  return json_encode($manifest);
}

function islandora_sc_get_book_object_manifest(AbstractObject $object) {
  global $base_url;
  $manifest = array(
    '@id' => $object->id,
    '@type' => 'sc:Manifest',
    'label' => $object->label,
    'metadata' => "$base_url/islandora/object/{$object->id}/datastream/MODS/view",
    'sequences' => array(
      array(
        '@type' => 'sc:Sequence',
        'label' => "{$object->label}, in order",
        'canvases' => array(),
      ),
    ),
  );
  return $manifest;
}

function islandora_sc_get_page_object_manifest(AbstractObject $object, array $info, $image_uri) {
  global $base_url;
  $manifest = array(
    '@type' => 'sc:Canvas',
    '@id' => $object->id,
    'label' => $object->label,
    'metadata' => "$base_url/islandora/object/{$object->id}/datastream/MODS/view",
    'images' => array(
      array(
        '@type' => 'oa:Annotation',
        'motivation' => 'sc:Painting',
        'resource' => array(
          '@id' => $image_uri,
          '@type' => 'dctypes:Image',
          'format' => $object['JP2']->mimetype,
          'height' => $info['height'],
          'width' => $info['width'],
        ),
      ),
    ),
  );
  return $manifest;
}
