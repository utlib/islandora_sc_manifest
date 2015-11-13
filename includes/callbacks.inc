<?php
/**
 * @file
 * Module callbacks.
 */

/**
 * SC manifest callback.
 */
function islandora_sc_manifest_callback(AbstractObject $object) {
  module_load_include('inc', 'islandora_sc_manifest', 'includes/shared_canvas_builder');
  drupal_json_output(islandora_sc_get_shared_canvas_manifest($object));
}
