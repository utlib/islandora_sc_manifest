<?php

/**
 * Creates the administration form 
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_mirador_bookreader administration form.
 */
function islandora_sc_manifest_admin_settings_form($form, &$form_state) {
  $get_default_value = function($name, $default) use($form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  //$solr_enabled = module_exists('islandora_solr');
  $form = array(
  'islandora_sc_manifest_image_uri' => array(
    '#type' => 'textfield',
    '#title' => t('SC Manifest Image URI'),
    '#default_value' => $get_default_value('islandora_sc_manifest_image_uri', 'https://iiif.library.utoronto.ca/image/v2/'),
  ),
  'islandora_sc_manifest_presentation_uri' => array(
    '#type' => 'textfield',
    '#title' => t('SC Manifest Presentation URI'),
    '#default_value' => $get_default_value('islandora_sc_manifest_presentation_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'),
  )
  );
  return system_settings_form($form);
}
